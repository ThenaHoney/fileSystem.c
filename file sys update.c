/**
 * Emulates a file system.
 *
 * @author Thomas Reichherzer
 */

#include "fileSystem.h"
#include "inode.h"
#include "disk.h"
#include "superBlock.h"

void readSuperBlock( superBlock_t* superBlock );
void writeSuperBlock( superBlock_t * superBlock );
void readInode( inode_t* inode, int blockNumber );
void writeInode( inode_t* inode, int blockNumber );
void printInodes();
void printFreeBlocks();
void intToByteArray( int value, char* buffer, int pos );
int byteArrayToInt( char* buffer, int pos );


/**
 * A test program for testing file system operations.
 *
 * @param args - a list of arguments
*/
int main(int argc, char* argv[])
{
	int error;
	superBlock_t superBlock;
	superBlock_t* spB = &superBlock;
	inode_t node;
	inode_t* nodep = &node;
	initializeInode(nodep);
	char path[25] = "storageDisk";
	if((error = format( 100000, path)))
	{
		printf("There was an error formatting the disk\n");
	}
	printf("\nHello World\n");
	//openDiskFile(path);
	readSuperBlock(spB);
	printSupBlock(spB);
	readInode(nodep, 1);
	printInodesTest(nodep);
	//closeDiskFile();
	return 0;
}

int format( int diskSizeInKB, char* path )
{
	int error, i;
    // create a disk
	if((error = createDisk(diskSizeInKB, path)))
	{
		printf("There was an error with creating the disk\n");
	}
	else
	{
    // complete implementation of this function
    // you must use the functions read/writeSuperblock() & read/writeInode()
	// formatting requires that you setup the superblock, write empty inodes,
	// and setup your free list for the remaining blocks
	superBlock_t supBlock;
	superBlock_t* spBlck = &supBlock;
	fd_t* fileTemp  = NULL;
	spBlck = intializeSupBlock(spBlck);
	inode_t* node;
	inode_t iNode[NUM_INODES];
	for(i = 0; i < NUM_INODES; i++)
	{
		initializeInode(&iNode[i]);
	}
	block_t* dataBlock = NULL;
	dataBlock = initializeBlockList(dataBlock);

	//read content to data blocks
	//write content to file or print out content
	//test if it prints out the content of the superBlock
	printSupBlock(spBlck);
	printInodesTest(&iNode[0]);
	writeSuperBlock(spBlck);
	node = &iNode[0];
	node->_flags = 4;
	node->_owner = 5;
	node->_filesize = 56;
	int i;
	for(i = 0; i < 12; i++)
	{
		node->_filename[i] = 2;
	}
	writeInode(node, 0);
	initializeFreeBlockList();
	//closeDiskFile();
	}


	return error;
}

/**
 * Opens a binary file on the disk for storing data.
 */
fd_t* openf(char* name)
{
	fd_t * temp = NULL;
	superBlock_t* spB;
	inode_t* nodep;
	readSuperBlock(spB);
	int i=0, j=0;
	while (i<(spB->_numberOfInodes))
	{
	//printSupBlock(spB);
	readInode(nodep, j);
	if (name == nodep->_filename)
		//Exist !! open it!!
	{
		temp->inodeBlockNum = j;
		temp->fileptr = nodep;
		//returns file descriptor
	return 	temp;
	}
	else
	{
		i++;
		j++;
	}
	}
	//printInodesTest(nodep);
	// file not found, create one
	writeSuperBlock(spB);
	writeInode(nodep,spB->_firstBlockOfFreeList);
	return temp;
}

int closef(fd_t *stream)
{

	if (stream)
		return NULL; //////////////////////dont have to return NULL
	else
		return -1;
}

int readf(fd_t* stream, char* data, int size)
{
	superBlock_t* spB;
	inode_t* nodep;
	if (stream)
	{
	readSuperBlock(stream->inodeBlockNum);
	readInode()
	return 0;
	}
	else
	return -1;
}

int writef(fd_t* stream, char* data, int size)
{
	superBlock_t* spB;
	inode_t* nodep;
	if (stream)
	{
	openf(stream);
	writeSuperBlock(spB);
	writeInode(nodep, spB->_firstBlockOfFreeList );
	return 0;
	}
	else
	return -1;
}

/**
    * Reads the super block from disk.
    *
    * @param - destination to store the disk's super block
    */
void readSuperBlock( superBlock_t* superBlock )
{
    int error;
	char buffer[BLOCK_SIZE];

    // read block and convert it to the superblock
	error = readBlock(0, buffer);
	if(error == -1)
	{
		printf("There was an error reading a block and converting it to the superBlock\n");
		printErrorDesc();
	}

//	int k, m;
//	for(k = 0; k < BLOCK_SIZE;k = k + 10)
//	{
//	 	for(m = 0; m < 10; m++)
//	   	{
//	   	printf("%d ", buffer[k]);
//	   	}
//	   printf("\n");
//	}

	//read number of the first free block
	int numFirstFree = byteArrayToInt(buffer, 8);
	superBlock->_firstBlockOfFreeList = numFirstFree;

	// read number of inodes from block
	int numInodes =  byteArrayToInt(buffer, 4 );
	superBlock->_numberOfInodes = numInodes;

	// read number of disk blocks from block
	int numDiskBlocks = byteArrayToInt(buffer, 0);
	superBlock->_numberOfDiskBlocks = numDiskBlocks;

//	int i;
//	for(i = 0; i < 4;i++)
//	{
//		printf("%x ", ((int)buffer[i] & 0xff));
//	}
//	printf("\n");
	// read start of free block to disk

	// finish implementing this function
}

/**
 * Writes the super block to disk.
 *
 * @param superBlock - the disk's super block
 */
void writeSuperBlock(superBlock_t* superBlock )
{
	int error = 0;
	// setup buffer to be written to disk
	char buffer[BLOCK_SIZE];
	clearBuffer(buffer);

	// write number of disk blocks into buffer
	intToByteArray(superBlock->_numberOfDiskBlocks, buffer, 0);

	// write number of inodes into buffer
	intToByteArray(superBlock->_numberOfInodes, buffer, 4);

	//write the number of the first free block
	intToByteArray(superBlock->_firstBlockOfFreeList, buffer, 8);

	printf("\nThe size of int: %d\n", (int)sizeof(int));
	printf("The size of char: %d\n", (int)sizeof(char));
	printf("The buffer is: \n");
	int i;
	for(i = 0; i < 4;i++)
	{
		printf("%x ", ((int)buffer[i] & 0xff));
	}
	printf("\n");
	i = 12;
	while(i < 254)
	{
		buffer[i] = 510;
		i = i + 4;
	}
	error = writeBlock(0, buffer, BLOCK_SIZE);
	if(error)
	{
		printErrorDesc();
	}

    // finish this function
}

/**
 * Reads an Inode from disk.
 *
 * @param inode - the inode to read into
 * @param blockNumber - the block number that holds the inode to be read
 *
*/
void readInode(inode_t* inode, int blockNumber)
{
	int error;
	char block[BLOCK_SIZE];
	// read block containing the Inode
    error = readBlock(blockNumber, block);

//    int k, m;
//    for(k = 0; k < BLOCK_SIZE;k = k + 10)
//    {
//    	for(m = 0; m < 10; m++)
//    	{
//    	printf("%d ", block[k]);
//    	}
//    	printf("\n");
//    }

    int flag = byteArrayToInt(block, 0);
    int owner = byteArrayToInt(block, 4);
    int fileSize = byteArrayToInt(block, 8);
    char name[12];
    int i, j = 12;
    for(i = 0; i < 12; i++)
    {
    	name[i] = block[j];
    	inode->_filename[i] = name[i];
    	j++;
    }

    inode->_flags  = flag;
    inode->_owner = owner;
    inode->_filesize = fileSize;


}

/**
 * Writes an Inode to disk.
 *
 * @param inode - the inode to be written to disk
 * @param blockNumber - the block number in which the inode will be written
 *
*/
void writeInode(inode_t* inode, int blockNumber)
{
	int error = 0;
	// implement this function
	char buffer[BLOCK_SIZE];
	clearBuffer(buffer);
	//write flags to buffer
	intToByteArray(inode->_flags, buffer, 0);

	//write owner to buffer
	intToByteArray(inode->_owner, buffer, 4);

	//write file size to buffer
	intToByteArray(inode->_filesize, buffer, 8);

	//write file name to buffer
	int i, j = 0;
	for(i = 12; i < 48; i = i + 4)
	{
		intToByteArray(inode->_filename[j], buffer, i);
		j++;
	}

	while(i < BLOCK_SIZE)
	{
		if(inode->pointer[i] != -1)
		{
			intToByteArray(inode->pointer[i], buffer, i);
		}
		else
		{
			intToByteArray(-1, buffer, i);
		}
		i = i + 4;
	}
	//while(i < MAX_POINTERS)
	error = writeBlock(16, buffer, BLOCK_SIZE);
	if(error)
	{
		printErrorDesc();
	}
}

/**
 * Prints the content of the Inodes of the virtual disk to the screen.
 */
void printInodes()
{
//	int error;
//	char buffer[BLOCK_SIZE];
//	fd_t t;
//	error = readBlock(1, buffer);
//
//	int numberOfInodes = byteArrayToInt(buffer, 0);
//	int i, j, k = 4;
//	while(i < numberOfInodes)
//	{
//		inode_t temp;
//		temp->_flags = byteArrayToInt(buffer, k);
//		k = k +4;
//		if(temp->_flags != 0)
//		{
//			temp->_owner = byteArrayToInt(buffer, k);
//			k = k + 4;
//			temp->_filesize = byteArrayToInt(buffer, k);
//			k = k + 4;
//			for(j = 0; j < 12; j++)
//			{
//				temp->_filename[i] = byteArrayToInt(buffer, k);
//				k = k + 4;
//			}
//			temp->pointer[0] = byteArrayToInt(buffer, k);
//			k = k + 4;
//			j = 0;
//			while((temp->pointer[j] != -1) && (j < MAX_POINTERS))
//			{
//
//			}
//		}
//		else
//		{
//
//		}
//	}
}

/**
 * Prints the block numbers of disk blocks that are free.
 */
void printFreeBlocks()
{
	// implement this function
	inode_t* nodep;
	int i;
	for(i=1;i<17;i++)
	{
		readInode(nodep,i);
		if (nodep->_flags == 0)
			//printf(""); printblock
	}

}


/**
 * Writes an integer to a byte array.
 *
 * @param value - the integer to be written to a byte array
 * @param buffer - the byte array in which the integer will be written
 * @param pos - the position in the byte array where an integer will be written
 */
void intToByteArray( int value, char* buffer, int pos )
{
	int i;

    for (i = 0; i < 4; i++)
	{
        int shift = (4 - 1 - i) * 8;
        buffer[i+pos] = (char) (( (unsigned int) value >> shift) & 0xFF);
    }
    //printf("\nadding value: %d to position %d", value, pos);
}

 /**
  * Reads an integer from a byte array at a specific location.
  *
  * @param buffer - the byte array from which an integer will be read
  * @param pos - the position in the array from where an integer will be read
  *
  * @return an integer read from the byte array
 */
int byteArrayToInt( char* buffer, int pos )
{
	int i, value = 0;

    for (i = 0; i < 4; i++)
	{
		int shift = (4 - 1 - i) * 8;
        value += (buffer[i + pos] & 0x000000FF) << shift;
    }
    return value;
}

superBlock_t * intializeSupBlock(superBlock_t* spBlck)
{
	spBlck->_numberOfDiskBlocks = 1012;
	spBlck->_numberOfInodes = NUM_INODES;
	spBlck->_firstBlockOfFreeList = 2;
	return spBlck;
}

void printSupBlock(superBlock_t* spBlck)
{
	printf("Number of DiskBlocks: %d\n", spBlck->_numberOfDiskBlocks);
	printf("Number of Inodes: %d\n", spBlck->_numberOfInodes);
	printf("First Block Free in the List: %d\n", spBlck->_firstBlockOfFreeList);
}

void printInodesTest(inode_t * iNode)
{
	printf("The Inode is:\n");
	printf("Flag: %d\n", iNode->_flags);
	printf("Owner: %d\n", iNode->_owner);
	printf("FileSize: %d\n", iNode->_filesize);
	printf("File Name: %s\nNodes:  ", iNode->_filename);
	int i = 0;
	while(iNode->pointer[i] != -1)
	{
		printf("%d ", iNode->pointer[i]);
		i++;
	}

	printf("\n\n");

}

void initializeFreeBlockList()
{
	int i, k = 2;
	fprintf(stderr, "This runs\n");
	for(i = 0; i < 2; i++)
	{
		char buffer[BLOCK_SIZE];
		int j;
		for(j = 0; j < BLOCK_SIZE; j++)
		{
			buffer[j] = 255;
		}
		writeBlock(k, buffer, BLOCK_SIZE);
		k++;
	}
}

void clearBuffer(char* buf)
{
	int i;
	for(i = 0; i < BLOCK_SIZE; i++)
	{
		buf[i] = 0;
	}
}
