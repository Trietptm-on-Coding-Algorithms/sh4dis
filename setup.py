import setuptools

with open("README.md", "r") as fh:
    long_description = fh.read()

setuptools.setup(
    name="sh4dis",
    version="1.0.0",
    author="Andrew Lamoureux",
    author_email="foo@bar.com",
    description="SH4 disassembler library",
    long_description=long_description, # load from README.md
    long_description_content_type="text/markdown",
    url="https://github.com/lwerdna/sh4dis",
    packages=setuptools.find_packages(),
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: Public Domain",
        "Operating System :: OS Independent",
    ],
)
