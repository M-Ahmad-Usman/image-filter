# Image Filter

## Description

This project is an image filter application written in C. It processes BMP images and applies various filters to them. The primary objective of this project is to parallelize the filters in `helpers.c` using OpenMP to reduce the execution time.

## Objective
he main objective of this project is to parallelize the image processing filters using OpenMP to reduce the execution time. The filters are implemented in helpers.c and are parallelized using OpenMP directives.

## Working

The project supports the following image filters:
- **Grayscale**: Converts the image to shades of gray.
- **Sepia**: Applies a sepia tone to the image.
- **Reflect**: Reflects the image horizontally.
- **Edges**: Detects edges in the image using the Sobel operator.

### How It Works

1. **Command-Line Arguments**: The program accepts command-line arguments to specify the filter type and input/output files.
2. **Reading BMP Files**: The program reads the BMP file headers and pixel data.
3. **Applying Filters**: Based on the specified filter, the corresponding function from `helpers.c` is called to process the image.
4. **Writing BMP Files**: The processed image is written to the output file.

### Building the Project

To build the project, run the following command:
```sh
make filter
```

To run the project, type:
```sh
./filter.out flag infile outfile
```
- ```flag```: Specifies the filter to apply (e for edges, g for grayscale, r for reflect, s for sepia).
- ```infile```: The input BMP file.
-```outfile```: The output BMP file.

## Performance Comparison
The performance comparison between the parallelized and sequential versions of the filters is given below:

| **Filters** 	| **Sequential Execution** 	| **Parallel Execution** 	|
|-------------	|--------------------------	|------------------------	|
| Grayscale   	| 0.0061584 seconds        	| 0.0011523 seconds      	|
| Sepia       	| 0.015165 seconds         	| 0.001125 seconds       	|
| Reflect     	| 0.0016543 seconds        	| 0.0002468 seconds      	|
| Edges       	| 0.0445879 seconds        	| 0.0125864 seconds      	|