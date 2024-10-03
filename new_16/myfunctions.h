
#if !defined(This_is_about_the_identifier_of_the_structure_DataOfFile__AroundTheWorld__C)
#define This_is_about_the_identifier_of_the_structure_DataOfFile__AroundTheWorld__C

typedef struct DataOfFile
{	
	unsigned int SizeOfFile;  // 文件大小 单位 字节

	_iobuf* FileStream;  // 源文件指针

	_iobuf* WriteFileStream;  // 目标文件指针

}Data;

#endif




Data* GetFileSize(char* PATH);  // 获取文件大小

char* RequestMemory(int fileSize);  // 根据输入的值申请内存

char* ReadFileToMemory(char* pMemory, unsigned int SizeOfFile,  unsigned int ReadSizeMax, _iobuf* pFileStream);
// 读取文件流的内容到指定的内存空间

void FreeMemory(char* pMemory); // 根据指针释放内存

int CloseFileStream(_iobuf* pFileOpen);  // 根据指针关闭文件流

int WriteMemoryToNewFile(char* pMemory, unsigned int SizeOfFile, unsigned int ReadSizeMax, char* PATH); 
 // 将流中的数据写入到硬盘的指定位置

void PrintFormatOfPe(unsigned int number, char* pMemoryAddress);  // 格式化打印PE文件的数据 