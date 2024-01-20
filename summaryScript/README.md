# Script Overview
This script processes `txt` files to analyze DeBruijn sequences, categorizing them based on complexity. It identifies sequences yielding and not yielding DeBruijn sequences, and generates three outputs for each complexity.

### Input
The tool takes a single `string` argument: the path to the head directory containing all the output files of the [computation tool](https://github.com/joannj35/debruijn-sequence-complexity-distribution).
* For the tool to identify the files and generate the overview summary, the file name ***MUST*** follow the specific string pattern: `"field_*_span*_complexity_*.txt"` where `*` is a wildcard character.

### Output 
For each file in the input directory, the script generates three types of output files:
1. Files containing sequences that yield DeBruijn sequences.
2. Files containing sequences that do not yield DeBruijn sequences.
3. Summary files that provide an overview of the sequences processed, including counts for each category.
* The tool runs a sanity check while processing the files and before prining the final results. Meaning the files will be generated only if the calculations are correct.

### Limitations
The script expects files to follow a specific naming and content format (as provided by the computation tool).
Existing output files for a given complexity level will ***not*** be overwritten. If output files already exist, the script will skip processing the corresponding input file.
