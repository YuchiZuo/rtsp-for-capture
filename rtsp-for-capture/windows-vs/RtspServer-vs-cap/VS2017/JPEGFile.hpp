#pragma once
#include "JPEGSource.h"
class JPEGFile
{
public:
	JPEGFile(int buf_size = 500000);
	~JPEGFile();

	bool Open(const char* path);
	void Close();

	bool IsOpened() const
	{
		return (m_file != NULL);
	}

	int ReadFrame(char* in_buf, int in_buf_size, bool* end);

private:
	FILE* m_file = NULL;
	unsigned char* m_buf = NULL;
	int  m_buf_size = 0;
	int  m_bytes_used = 0;
	int  m_count = 0;
};


JPEGFile::JPEGFile(int buf_size)
	: m_buf_size(buf_size)
{
	m_buf = new unsigned char[m_buf_size];
}

JPEGFile::~JPEGFile()
{
	delete[] m_buf;
}

bool JPEGFile::Open(const char* path)
{
	m_file = fopen(path, "rb");
	if (m_file == NULL) {
		return false;
	}

	return true;
}

void JPEGFile::Close()
{
	if (m_file) {
		fclose(m_file);
		m_file = NULL;
		m_count = 0;
		m_bytes_used = 0;
	}
}

int JPEGFile::ReadFrame(char* in_buf, int in_buf_size, bool* end)
{
	if (m_file == NULL || m_buf == NULL) {
		return -1;
	}
	int i = 0;
	int bytes_read = (int)fread(m_buf, 1, m_buf_size, m_file);
	if (bytes_read == 0) {
		fseek(m_file, 0, SEEK_SET);
		m_count = 0;
		m_bytes_used = 0;
		bytes_read = (int)fread(m_buf, 1, m_buf_size, m_file);
		if (bytes_read == 0) {
			this->Close();
			printf("error read file return zero\n");
			return -1;
		}
	}
	//printf("bytes_read = %d\n", bytes_read);
#if 0
	//for pic
	int size = (bytes_read <= in_buf_size ? bytes_read : in_buf_size);
	memcpy(in_buf, m_buf, size);
	fseek(m_file, 0, SEEK_SET);
	return bytes_read;
#else
	//for mjpeg
	bool is_find_start = false;
	bool is_find_end = false;
	unsigned int start_pos = 0;
	unsigned int end_pos = 0;
	*end = false;
	//printf("0x%x 0x%x\n", m_buf[0], m_buf[1]);
	for (i = 0; i < bytes_read - 1; i++) {
		unsigned char cur = m_buf[i + 0];
		unsigned char next = m_buf[i + 1];

		if (is_find_start == false && cur == 0xff && next == 0xd8) {
			is_find_start = true;
			start_pos = i;
			printf("start_pos = %d\n", start_pos);
			continue;
		}
		if (is_find_start == true && cur == 0xff && next == 0xd9) {//EOI
			is_find_end = true;
			end_pos = i + 2;
			printf("end_pos = %d\n", end_pos);
			break;
		}
		else if (is_find_start == true && cur == 0xff && next == 0xd8) {//
			is_find_end = true;
			end_pos = i;
			printf("end_pos = %d\n", end_pos);
			break;
		}
	}
	//printf("-----------------------\n");
	if (is_find_start && is_find_end) {

	}

	if (is_find_start && !is_find_end) {
		is_find_end = true;
		end_pos = i + 1;
		//*end = true;
		//printf("!!end_pos = %d\n", end_pos);
	}

	if (!is_find_start || !is_find_end) {
		m_count = 0;
		m_bytes_used = 0;
		printf("check your file, no start or end flag\n");
		this->Close();
		return -1;
	}

	int start_end_len = end_pos - start_pos;
	int size = (start_end_len <= in_buf_size ? start_end_len : in_buf_size);
	//printf("start_end_len = %d, size = %d\n", start_end_len, size);
	memcpy(in_buf, &m_buf[start_pos], size);
	m_bytes_used += size;
	fseek(m_file, m_bytes_used, SEEK_SET);
	return size;
#endif
}


