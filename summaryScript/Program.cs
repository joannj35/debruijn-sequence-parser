using System.Text;
using System.Text.RegularExpressions;

namespace parserExtentions
{
    class Program
    {
        /// <summary>  file name pattern  </summary>
        static string pattern = "field_*_span*_complexity_*.txt";
        /// <summary> head direcotry which includes all the input files </summary>
        static string directoryPath = string.Empty;

        static void Main(string[] args)
        {
            Console.WriteLine("Enter the head direcotry which includes all the input files (data folder)");
            var dataDirectory = Console.ReadLine();
            if (dataDirectory == null)
                return;

            directoryPath = Path.Combine(dataDirectory);
            try
            {
                var files = Directory.EnumerateFiles(directoryPath, pattern, SearchOption.AllDirectories);
                Parallel.ForEach(files, (file) =>
                {
                    ProcessFile(file);
                });
            }
            catch (Exception e)
            {
                Console.WriteLine($"Error: {e.Message}");
            }
        }
        static void ProcessFile(string filePath)
        {
            string ComplexityDirPath = Path.GetDirectoryName(filePath) ?? throw new Exception($"Path directory returned null"); // complexity direcotry path
            string fileContent = File.ReadAllText(filePath);
            string pattern = @"(total number of sequences of small complexity \d+ is: \d+|total number of debruijn sequences of complexity \d+ is: \d+)";

            GetValueBasedOnRegex(ComplexityDirPath, @"F_(\d+)", out int field); //assuming directories are like: F_%/span_%/%
            GetValueBasedOnRegex(ComplexityDirPath, @"span_(\d+)", out int span); //assuming directories are like: F_%/span_%/%
            GetValueBasedOnRegex(fileContent, @"complexity (\d+)\s*:", out int complexity);

            // output file paths
            string yieldingFilePath = Path.Combine(ComplexityDirPath, $"{complexity}_yielding_small_sequences.txt"); //small sequences only
            string nonYieldingFilePath = Path.Combine(ComplexityDirPath, $"{complexity}_non_yielding_small_sequences.txt");
            string summaryFilePath = Path.Combine(ComplexityDirPath, $"{complexity}_summary.txt");
            string filteredFilePath = Path.Combine(ComplexityDirPath, $"{complexity}_yielding_sequences.txt"); // small sequences and their yielded deBruijns

            // skip processing if files already exist
            if (File.Exists(yieldingFilePath) || File.Exists(nonYieldingFilePath) || File.Exists(summaryFilePath))
            {
                Console.WriteLine($"Skipping file processing as output files for complexity {complexity} already exist.");
                return;
            }

            try
            {
                using (StreamWriter nonZeroWriter = File.AppendText(yieldingFilePath))
                using (StreamWriter zeroWriter = File.AppendText(nonYieldingFilePath))
                using (StreamWriter summaryWriter = File.AppendText(summaryFilePath))
                using (StreamWriter yieldingWriter = File.AppendText(filteredFilePath))
                {
                    foreach (Match match in Regex.Matches(fileContent, pattern))
                        summaryWriter.WriteLine(match.Value);
                    
                    //calculate small complexity
                    GetSmallSeq(fileContent, out List<string> yieldDb, out List<string> nonYieldDb);
                    var smallComplexity = field == 2 ? (complexity - Math.Pow(field, span - 1)) : (complexity % field);

                    //summary overview
                    summaryWriter.WriteLine($"The total number of sequences of small complexity={smallComplexity} which yield debruijn sequences is: {yieldDb.Count}\n"
                                  + $"The total number of sequences of small complexity={smallComplexity} which DO NOT yield any debruijn sequences is: {nonYieldDb.Count}");

                    //yielding small sequences
                    nonZeroWriter.WriteLine($"Summary of sequences of small complexity={smallComplexity} which yield debruijn sequences:\n"
                        + $"Total number of sequences in this file: {yieldDb.Count}");
                    foreach (var seq in yieldDb)
                        nonZeroWriter.WriteLine($"{seq}");

                    //non yielding small sequnences
                    zeroWriter.WriteLine($"Summary of sequences of small complexity={smallComplexity} which DO NOT yield debruijn sequences:\n"
                        + $"Total number of sequences in this file: {nonYieldDb.Count}");
                    foreach (var seq in nonYieldDb)
                        zeroWriter.WriteLine($"{seq}");

                    //yielding small sequences and their yielded deBruijns
                    FilterYieldingSequences(fileContent, out string filteredFileContent, out int capturedSequencesNum);
                    yieldingWriter.WriteLine(filteredFileContent);

                    // sanity check
                    GetValueBasedOnRegex(fileContent, @"of small complexity \d+ is: (\d+)", out int total);
                    if (nonYieldDb.Count + yieldDb.Count != total || capturedSequencesNum != yieldDb.Count)
                        throw new Exception($"Sanity check for complexity {complexity} has failed! No summary files will be created.");

                    Console.WriteLine($"Processed file of complexity {complexity}");
                }
            }
            catch (Exception ex)
            {
                DeleteFilesForComplexity(ComplexityDirPath, complexity);
                Console.WriteLine($"Error: {ex.Message}");
            }
        }

        /// <summary> extracts an integer value based on a regex pattern. </summary>
        /// <param name="search"> content to extract values from</param>
        /// <param name="pattern"> regex pattern to match </param>
        /// <param name="value"> out value variable </param>
        static void GetValueBasedOnRegex(string search, string pattern, out int value)
        {
            var match = Regex.Match(search, pattern);
            value = match.Success ? int.Parse(match.Groups[1].Value) : -1;
        }

        /// <summary> Extracts SMALL sequences that yield and do not yield DeBruijn sequences from file </summary>
        /// <param name="yieldDb">list of sequences of small complexity which yield debruijn sequences</param>
        /// <param name="nonYieldDb">list of sequences of small complexity which DIDNT yield debruijn sequences</param>
        static void GetSmallSeq(string fileContent, out List<string> yieldDb, out List<string> nonYieldDb)
        {
            yieldDb = new List<string>();
            nonYieldDb = new List<string>();

            string pattern = @"Debruijn Sequences generated by the sequence (\d+) : [\s\S]*?the number of Debruijn sequences: (\d+)";

            var matchCollection = Regex.Matches(fileContent, pattern);
            foreach (Match match in matchCollection)
            {
                if (match.Groups[2].Value == "0")
                    nonYieldDb.Add(match.Groups[1].Value);
                else
                    yieldDb.Add(match.Groups[1].Value);
            }
        }

        /// <summary> Extracts yielding small sequences AND their yielded deBruijns  </summary>
        /// <param name="filteredFileContent"> the filtered file content </param>
        /// <param name="yieldingSequencesNum"> the amount of yielding small sequences captured by the regex (used for sanity check) </param>
        static void FilterYieldingSequences(string fileContent, out string filteredFileContent, out int capturedSequencesNum)
        {
            string pattern1 = @"For order \d+ complexity \d+\s?:";
            string pattern2 = @"Debruijn Sequences generated by the sequence \d+ : [\s\S]*?the number of Debruijn sequences: (\d+)";
            StringBuilder sb = new StringBuilder();
            capturedSequencesNum = 0;

            var matchLine = Regex.Match(fileContent, pattern1);
            sb.AppendLine(matchLine.Value + "\n");

            var matchCollection = Regex.Matches(fileContent, pattern2);
            
            if(matchCollection.Count == 0)
            {
                sb.AppendLine("The amount of yielded deBruijn sequences is 0"); // Append the whole match
            }
            else
            {
                foreach (Match match in matchCollection)
                {
                    if (match.Groups[1].Value != "0")
                    {
                        sb.AppendLine(match.Value + "\n"); // Append the whole match
                        capturedSequencesNum++; // for sanity check later
                    }
                }
            }

            filteredFileContent = sb.ToString();
        }
        /// <summary>
        /// in case of an error, the corrupted files of the specified complexities are deleted
        /// </summary>
        static void DeleteFilesForComplexity(string directoryPath, int complexity)
        {
            var filesToDelete = Directory.GetFiles(directoryPath, $"{complexity}_*.txt");
            foreach (var file in filesToDelete)
            {
                File.Delete(file);
            }
        }
    }
}