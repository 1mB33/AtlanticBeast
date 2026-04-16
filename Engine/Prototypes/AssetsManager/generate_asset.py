import io
import os
import sys
import zlib


def print_help() -> None:
    print("""
          TODO: Write help message
    """)


def validadte_and_process_file_type(fileType: str) -> str:
    if fileType.lower() == "mesh":
        return "_Mesh"

    raise Exception(f"Unkonwn fileType: {fileType}")


def construct_file_name(name: str, fileType: str) -> str:
    return name + validadte_and_process_file_type(fileType) + ".b33asset"


def process_obj_file(file: io.BufferedWriter) -> bytes:
    return file.read()


def process_file(file: io.BufferedWriter, fileType: str, ext: str) -> bytes:
    if ext == ".obj":
        if fileType != "mesh":
            raise Exception(f"Extention doesn't match the requested file type: {ext}, {fileType}")
        return process_obj_file(file)
    raise Exception(f"Unkonwn extention: {ext}")


if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Not enough arguments provided")
        print_help()
        exit(-1)
    try:
        sourceFileName = os.path.splitext(sys.argv[1])[0]
        sourceFileExt = os.path.splitext(sys.argv[1])[1]
        destFileType = sys.argv[2]
        with open(sys.argv[1], "rb") as sourceFile:
            with open(construct_file_name(sourceFileName, destFileType), "wb") as destFile:
                compressed = zlib.compress(process_file(sourceFile, destFileType, sourceFileExt), level=zlib.Z_BEST_COMPRESSION, wbits=-15)
                destFile.write(compressed)
                print("Compressed String:", compressed)
    except Exception as e:
        print(str(e))
