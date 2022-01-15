# virtual-file-storage
Virtual file storage system with a few familiar commands built in

# Commands:
mkdir, touch, ls, exit, rm, start, fsio, extstart

# Custom Commands:

## fsio
Description:
Input and output command for files.

Example:
> fsio file.txt write 'Hello, World!'\
> fsio file.txt write 'The quick brown fox jumps over the lazy dog.'\
> echo .... \
> fsio file.txt read

Result:
> .... \
> Hello, World! \
> The quick brown fox jumps over the lazy dog

## extstart
Description:
Starts external scripts.

script.sh:
> echo I am making a directory \
> mkdir dir \
> echo Directory made!

Example:
> extstart script.sh

Result:
> I am making a directory! \
> Directory made!
