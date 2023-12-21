# Debruijn Sequence Parser
[![License](https://img.shields.io/badge/license-Apache_2.0-green)](https://github.com/joannj35/debruijn-sequence-parser/blob/main/LICENSE)
[![tool](https://img.shields.io/badge/source_code-computational_tool-8A2BE2)](https://github.com/joannj35/debruijn-sequence-complexity-distribution)

## Overview
In this project we continue the study of the linear complexity of de Bruijn sequences over finite fields. Our aim is to establish the integer values of
linear complexity for which there exist de Bruijn sequences of given span over finite fields.

The repository houses a user-friendly GUI tool designed to visualize the result data of our study, thereby offering a more tangible grasp of the complexity distributions. For the source code of the computational tool that generated the results, please visit the [tool repository](https://github.com/joannj35/debruijn-sequence-complexity-distribution).

## Data Provided
The tool is preloaded with a diverse set of data for analyzing de Bruijn sequences across various fields and spans. The datasets are organized as follows:
```
data/
├── binary_field/
│   ├── sequences_of_small_complexity/   # sequences of which we generate the larger debruijn sequences
│   |   └── complexities_6-24/
│   ├── span_7/
│   |    └── complexities_71-88/
|   |       ├── summary_file
|   |       ├── non_yielding_small_sequences # sequences of small complexity which DONT yield debruijn sequences
|   |       └── yielding_small_sequences     # sequences of small complexity which yield debruijn sequences
│   └── span_6/
│       └── complexities_38-56/
|           └── ...
├── non_binary_field_3/
│   └── span_2/
│       ├── complexity_7/
|       |   └── ...
│       └── complexity_8/
|           └── ...
├── non_binary_field_5/
│   └── span_2/
│       ├── complexity_11/
|       |   └── ...
│       └── complexity_12/
|           └── ...
├── non_binary_field_7/
│   └── span_2/
│       ├── complexity_15/
|       |   └── ...
│       └── complexity_16/
|           └── ...
```

## Quick Start
1. Download the installer from the releases page [here](https://github.com/joannj35/debruijn-sequence-parser/releases) 
2. Run the installer and follow the on-screen instructions to complete the installation.
3. Launch the De Bruijn Sequence Parser app from your desktop or start menu.

## Contributing
Contributions to this project are highly appreciated. If you have suggestions or improvements, please fork the appropriate branch of the repository, make your changes, and submit a pull request.

## Support
If you encounter any issues or require assistance, please open an issue on the repository's [issue page](https://github.com/joannj35/debruijn-sequence-parser/issues).

## LICENSE
This project is licensed under Apache License 2.0. For more information, see the LICENSE file in this repository.
