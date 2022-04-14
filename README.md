# Image Compressor
A Unix command line tool for BMP and TIFF file compression

## Build
```bash
cd image-compressor
make dependencies
make
```
## Usage

All results of the image compressor are stored in `/home/user/image-compressor/` and are organized by the program start time.

Inside each timestamp folder are /inbound and /outbound folders containing the input and output files, respectively.

#### Single File
To compress one file: `compress.exe -f "/path/to/file.bmp"`
  
To decompress one file: `compress.exe -f "/path/to/compressed_file.jcif"`
  * The default path of a compressed file is: `/home/user/image-compressor/{timestamp}/outbound/{file}.jcif`

You can call `compress.exe` without a flag and the program will prompt you for a file, or multiple files separated by commas.

#### Batch Files
You can also provide the path to a batch file. A batch file is a text file that contains file paths for all of the to-be compressed or decompressed files, separated by newlines.

To compress or decompress multiple files using a batch file: `compress.exe -b "/path/to/batch_file.txt"` 
  * You can include a mix of compressed and non-compressed files in a batch file

#### JCIF Files
JCIF files are files that have been encoded/compressed by this program. When the program encounters a `.jcif` file, it will attempt to decode/decompress it.

### Limitations 
* Currently only supports reading and writing files from and to a Unix-based file system.
* Only supports Huffman coding for BMP and TIFF files.

### Examples

* **Compression**
  ```bash
  username@hostname:~/projects/image-compressor$ build/./compress.exe -file=./images/space.bmp

  Execution Time (MIN:SEC:MS): 0:0:369 | /home/user/images/space.bmp | Encoded | Initial File Size (Byes): 720662 | New File Size (Bytes): 659363 |
  ```

  ```bash
  username@hostname:~/projects/image-compressor$ build/./compress.exe -b=./images/batches/batch.txt

  Execution Time (MIN:SEC:MS): 0:1:355 | /path/one/to/test1.bmp | Encoded | Initial File Size (Byes): 3686538 | New File Size (Bytes): 2561737 | 
  Execution Time (MIN:SEC:MS): 0:0:376 | /path/two/to/space.bmp | Encoded | Initial File Size (Byes): 720662  | New File Size (Bytes): 659363  | 
  .jpg compression is not supported
  Batch completed successfully, however 1 files were unable to be compressed
  ```

  ```bash
  username@hostname:~/projects/image-compressor$ build/./compress.exe
  
  Enter the path of valid file(s) to be compressed. Separate multiple files by a comma (,): ./images/space.bmp, ./images/test1.bmp
  Execution Time (MIN:SEC:MS): 0:0:370 | /home/user/images/space.bmp | Encoded | Initial File Size (Byes): 720662  | New File Size (Bytes): 659363  | 
  Execution Time (MIN:SEC:MS): 0:1:340 | /home/user/images/test1.bmp | Encoded | Initial File Size (Byes): 3686538 | New File Size (Bytes): 2561737 |
  ```
  
* **Decompress**
  ```bash
  username@hostname:~/projects/image-compressor$ build/./compress.exe -file=home/user/image-compressor/2022-4-14T15:13:41/outbound/space.jcif

  Execution Time (MIN:SEC:MS): 0:0:350 | /home/user/image-compressor/2022-4-14T15:13:41/outbound/space.jcif | Decoded | Initial File Size (Byes): 659363 | New File Size (Bytes): 720054 | 
  ```

The minor difference between original file size and the decompressed file size is due to the fact that the image compressor disregards header data not needed to reconstruct the original image when decoding. 

## Program Flags
* -b, --batch=BATCHFILE                Text file(.txt) of files to be compressed. Separated by newlines.
* -f, --file=FILEPATH                  File path of the file to be compressed.
* -h, --help                           Print this message and exit.

## Future Improvements
* Optimize compression/decompression speed
* Further optimize size of the compressed file 
* Include support for more file types
* Include support for additional coding methods (e.g Shannon coding)
* Add support for user configuration: which coding method to use, where to store the compressed files, etc.
