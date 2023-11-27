
import subprocess
from subprocess import Popen, PIPE, STDOUT
import sys
import os
import argparse
from sys import platform

def main():
    # Set the working directory to the engine root folder
    apath = os.path.abspath(__file__)
    dirname = os.path.dirname(apath)
    os.chdir(dirname)
    os.chdir("..")

    subprocess.check_call([sys.executable, "-m", "venv", "env"])

    python_exe = ""
    conan_exe = ""

    if platform == "linux" or platform == "linux2" or platform == "darwin":
        python_exe = "env/Scripts/python"
        conan_exe = "env/Scripts/conan"
    elif platform == "win32":
        python_exe = "env/Scripts/python.exe"
        conan_exe = "env/Scripts/conan.exe"

    subprocess.check_call([python_exe, "-m", "pip", "install", "conan"])

    try:
        subprocess.check_call([conan_exe, "profile", "detect"])
    except:
        print("You can ignore this error.")

    try:
        subprocess.check_call([conan_exe, "install", ".", "--output-folder=build", "--build=missing", "--settings=build_type=Debug"])
    except subprocess.CalledProcessError:
        print("Error: Failed to install dependencies. Abort.")
        exit()

    print("Info: Successfully installed all dependencies. Ready for generating project files.")

if __name__ == "__main__":
    main()