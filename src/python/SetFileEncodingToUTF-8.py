#!/usr/bin/python
import os
import chardet

def detect_encoding(file_path):
    with open(file_path, 'rb') as file:
        raw_data = file.read()
        result = chardet.detect(raw_data)
        return result['encoding']

def convert_to_utf8(file_path, original_encoding):
    try:
        with open(file_path, 'r', encoding=original_encoding) as file:
            content = file.read()
        with open(file_path, 'w', encoding='utf-8') as file:
            file.write(content)
        print(f"Converted {file_path} from {original_encoding} to UTF-8")
    except Exception as e:
        print(f"Error converting {file_path}: {e}")

def convert_files_in_directory(directory_path):
    extensions = ['.java', '.c', '.cpp', '.txt', '.py', '.csv']
    for root, dirs, files in os.walk(directory_path):
        for file in files:
            file_path = os.path.join(root, file)
            if any(file_path.endswith(ext) for ext in extensions):
                original_encoding = detect_encoding(file_path)
                convert_to_utf8(file_path, original_encoding)

if __name__ == '__main__':
	print("--- 批量更改文件字符集编码 ---")
    input_directory = input("请输入目录路径：")
    convert_files_in_directory(input_directory)
