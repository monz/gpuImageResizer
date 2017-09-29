# gpuImageResizer
GPU accelerated image resizer

# Examples

## Windows
resize all `.jpg` files also in all subdirectories within `C:\tmp` using `5 threads` with new `width=1440` and `height=1080`. Resized images are saved in `output_directory`

```
> dir C:\tmp\*.jpg /A /S /B" | resizeOnGpu.exe -o C:\output_directory -t 5 -w 1440 -h 1080
```

> ## Note:
> Threads are used to simultaniously load images into the GPU, keeping the GPU busy. This is necessary due to small file sizes. Recommend 2 threads for non SSD.

> ## Warning:
> More threads requires faster source/destination hard drive!


## Known Issues
- new image width AND height must be given (not calculated automatically if one size is given)
- crash on non image files
- ...

## Dependencies
- OpenCV 3.3.0 or higher - http://opencv.org/
- CUDA Toolkit 8 or higher - https://developer.nvidia.com/cuda-toolkit

**Work in progress...**
