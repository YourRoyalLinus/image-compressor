#include "../../../include/Decoding/Huffman/HuffmanDecodingStrategy.h"
#include "../../../include/Decoding/Decoder.h"
#include "../../../include/FileHandling/FileMarshaller.h"
#include "../../../include/ImageHandling/ImageParser.h"

void HuffmanDecodingStrategy::Decode(File& currentFile){
    JCIFImage encodedImage(currentFile.fullPath.c_str()); //FILEMARSHALLER SHOULD WRAP ALL CALLS TO SINGLETONS
    ImageParser::instance().ParseImage(encodedImage);

    {
        std::shared_ptr<std::ifstream> encodedImageStream = Decoder::GetDecodedFileStream(currentFile);
        std::shared_ptr<HuffmanTreeNode> deserializedRootNode = Decoder::DeserializeFileData(*encodedImageStream, encodedImage.header->size);  

        BinaryReader binReader = BinaryReader(encodedImageStream, encodedImage.encodedPixelArrayBytes, encodedImage.bitPadding);

        std::vector<unsigned char> decodedPixelVec = Decoder::DecodePixelArray(binReader, deserializedRootNode);
        Decoder::CreateImageFile(decodedPixelVec, currentFile, encodedImage);
    }
    
    currentFile.size = FileMarshaller::instance().GetFileSize(currentFile.fullPath);
}
