#include "md5.h"

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <fstream>
#include <string>
#include <map>
#include <set>
//#include <pthread.h>

#define READ_DATA_SIZE	1024
#define MD5_SIZE		16
#define MD5_STR_LEN		(MD5_SIZE * 2)

// function declare
int Compute_file_md5(const char *file_path, char *md5_str);
void Check_in_map(std::string&, std::string& file_path, std::map<std::string, std::string>& md5_container, std::set<std::string>& repeat_set);

/************** main test **************/
int main(int argc, char *argv[])
{

    std::map<std::string, std::string> md5_container;
    std::set<std::string> repeat_set;
	int ret;
	char md5_str[MD5_STR_LEN + 1];
    std::string true_md5_str;
    std::string file_path;
    std::ifstream img_reader;
    img_reader.open(argv[1], std::ios::in);
    while (img_reader >> file_path){
        ret = Compute_file_md5(file_path.c_str(), md5_str);
        true_md5_str = md5_str;
        Check_in_map(true_md5_str, file_path, md5_container, repeat_set);

        //if (0 == ret)
        //{
            //printf("[file - %s] md5 value:\n", file_path.c_str());
            //printf("%s\n", md5_str);
        //}
    }
    img_reader.close();
    for (auto p=repeat_set.begin(); p != repeat_set.end(); ++p){
        printf((*p).c_str());
        printf("\n");
    }
	return 0;
}

void Check_in_map(std::string& true_md5_str, std::string& file_path, std::map<std::string, std::string>& md5_container, std::set<std::string>& repeat_set){
    std::map<std::string, std::string>::iterator pt = md5_container.find(true_md5_str);
    if (pt != md5_container.end()){
        repeat_set.insert((*pt).second);
        repeat_set.insert(file_path);
    }
    md5_container.insert(std::pair<std::string, std::string>(true_md5_str, file_path));
}

/**
 * compute the value of a file
 * @param  file_path
 * @param  md5_str
 * @return 0: ok, -1: fail
 */
int Compute_file_md5(const char *file_path, char *md5_str)
{
	int i;
	int fd;
	int ret;
	unsigned char data[READ_DATA_SIZE];
	unsigned char md5_value[MD5_SIZE];
	MD5_CTX md5;

	fd = open(file_path, O_RDONLY);
	//if (-1 == fd)
	//{
		//perror("open");
		//return -1;
	//}

	// init md5
	MD5Init(&md5);

	while (1)
	{
		ret = read(fd, data, READ_DATA_SIZE);
		//if (-1 == ret)
		//{
			//perror("read");
			//close(fd);
			//return -1;
		//}

		MD5Update(&md5, data, ret);

		if (0 == ret || ret < READ_DATA_SIZE)
		{
			break;
		}
	}

	close(fd);

	MD5Final(&md5, md5_value);

	// convert md5 value to md5 string
	for(i = 0; i < MD5_SIZE; i++)
	{
		snprintf(md5_str + i*2, 2+1, "%02x", md5_value[i]);
	}

	return 0;
}

