﻿using System.Text.RegularExpressions;

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
            directoryPath = Path.Combine(args[0]);
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
            string spanDirPath = Path.GetDirectoryName(ComplexityDirPath) ?? throw new Exception($"Path directory returned null"); // complexity direcotry path
            int span = int.Parse(spanDirPath.Split('_')[1]); //assuming directories are like: F_%/span_%/%
            
            GetValueBasedOnRegex(fileContent, @"complexity (\d+):", out int complexity);

            // output file paths
            string yieldingFilePath = Path.Combine(ComplexityDirPath, $"{complexity}_yielding_small_sequences.txt");
            string nonYieldingFilePath = Path.Combine(ComplexityDirPath, $"{complexity}_non_yielding_small_sequences.txt");
            string summaryFilePath = Path.Combine(ComplexityDirPath, $"{complexity}_summary.txt");

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
                {
                    foreach (Match match in Regex.Matches(fileContent, pattern))
                        summaryWriter.WriteLine(match.Value);

                    GetSmallSeq(fileContent, out List<string> yieldDb, out List<string> nonYieldDb);

                    summaryWriter.WriteLine($"The total number of sequences of small complexity={complexity - Math.Pow(2, span - 1)} which yield debruijn sequences is: {yieldDb.Count}\n"
                                  + $"The total number of sequences of small complexity={complexity - Math.Pow(2, span - 1)} which DO NOT yield any debruijn sequences is: {nonYieldDb.Count}");

                    nonZeroWriter.WriteLine($"Summary of sequences of small complexity={complexity - Math.Pow(2, span - 1)} which yield debruijn sequences:\n"
                        + $"Total number of sequences in this file: {yieldDb.Count}");
                    foreach (var seq in yieldDb)
                        nonZeroWriter.WriteLine($"{seq}");

                    zeroWriter.WriteLine($"Summary of sequences of small complexity={complexity - Math.Pow(2, span - 1)} which DO NOT yield debruijn sequences:\n"
                        + $"Total number of sequences in this file: {nonYieldDb.Count}");
                    foreach (var seq in nonYieldDb)
                        zeroWriter.WriteLine($"{seq}");

                    // sanity check
                    GetValueBasedOnRegex(fileContent, @"of small complexity \d+ is: (\d+)", out int total);
                    if (nonYieldDb.Count + yieldDb.Count != total)
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
        /// <param name="fileContent"> file content to extract values from</param>
        /// <param name="pattern"> regex pattern to match </param>
        /// <param name="value"> out value variable </param>
        static void GetValueBasedOnRegex(string fileContent, string pattern, out int value)
        {
            var match = Regex.Match(fileContent, pattern);
            value = match.Success ? int.Parse(match.Groups[1].Value) : -1;
        }

        /// <summary> Extracts sequences that yield and do not yield DeBruijn sequences from file </summary>
        /// <param name="yieldDb">list of sequences of small complexity which yield debruijn sequences</param>
        /// <param name="nonYieldDb">list of sequences of small complexity which DIDNT yield debruijn sequences</param>
        static void GetSmallSeq(string fileContent, out List<string> yieldDb, out List<string> nonYieldDb)
        {
            yieldDb = new List<string>();
            nonYieldDb = new List<string>();

            string pattern = @"Debruijn Sequences generated by the sequence (\d+) : [\s\S]*?the number of Debruijn sequences: (\d+)";

            foreach (Match match in Regex.Matches(fileContent, pattern))
            {
                if (match.Groups[2].Value == "0")
                    nonYieldDb.Add(match.Groups[1].Value);
                else
                    yieldDb.Add(match.Groups[1].Value);
            }
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