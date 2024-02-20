# Debruijn Sequence Parser
[![License](https://img.shields.io/badge/license-Apache_2.0-green)](https://github.com/joannj35/debruijn-sequence-parser/blob/master/LICENSE)
[![tool](https://img.shields.io/badge/source_code-computational_tool-8A2BE2)](https://github.com/joannj35/debruijn-sequence-complexity-distribution)
[![data](https://img.shields.io/badge/data-overview-blue)](https://github.com/joannj35/debruijn-sequence-parser/blob/master/data/README.md)

## Overview
In this project we continue the study of the linear complexity of de Bruijn sequences over finite fields. Our aim is to establish the integer values of
linear complexity for which there exist de Bruijn sequences of given span over finite fields.

The repository houses a user-friendly GUI tool designed to visualize the result data of our study, thereby offering a more tangible grasp of the complexity distributions. For the source code of the computational tool that generated the results, please visit the [tool repository](https://github.com/joannj35/debruijn-sequence-complexity-distribution).

## Datasets
The tool is preloaded with a diverse set of data for analyzing de Bruijn sequences across various fields and spans. The datasets are organized as follows:
```
data/
├── binary_field/
│   ├── sequences_of_small_complexity/   # sequences of which we generate the larger debruijn sequences from
│   |   └── complexities_6-24/
|   |       └── ...
|   |
│   ├── span_6/
│   |   └── complexities_38-56/
|   |       ├── field_*_span_*_complexity_*  # output file of the computation tool
|   |       ├── summary_file                 # overview file for a specific complexity
|   |       ├── non_yielding_small_sequences # sequences of small complexity which DONT yield debruijn sequences
|   |       └── yielding_small_sequences     # sequences of small complexity which yield debruijn sequences
|   |
|   └── span_7/
│       └── complexities_71-88/
|           └── ...
|
├── non_binary_field_3/
│   ├── sequences_of_small_complexity/ 
│   |   └── complexities_1-2/
|   |
│   └── span_2/
│       └── complexities_7-8/
|           └── ...
|
├── non_binary_field_5/
│   ├── sequences_of_small_complexity/ 
│   |   └── complexities_1-5/
|   |
│   └── span_2/
│       └── complexities_11-24/
|           └── ...
|
├── non_binary_field_7/
│   ├── sequences_of_small_complexity/ 
│   |   └── complexities_1-7/
|   |       └── ...
|   |
│   └── span_2/
│       └── complexities_15-25/
|           └── ...
```

## Quick Start
1. Download the installer from the releases page [here](https://github.com/joannj35/debruijn-sequence-parser/releases) 
2. Run the installer and follow the on-screen instructions to complete the installation.
3. Launch the De Bruijn Sequence Parser app from your desktop or start menu.

## Citation
If you use this software or the [data it produces](https://github.com/joannj35/debruijn-sequence-parser/tree/master/data) in your work,  please cite it using the following reference:

Etzion, T., Jozen, J., & Dally, R. (2024). debruijn-sequence-complexity-distribution (Version 1.0.0) [Computer software]. https://doi.org/10.5281/zenodo.10685239

## LICENSE
This project is licensed under Apache License 2.0. For more information, see the LICENSE file in this repository.

## Support
If you encounter any issues or require assistance, please open an issue on the repository's [issue page](https://github.com/joannj35/debruijn-sequence-parser/issues).
