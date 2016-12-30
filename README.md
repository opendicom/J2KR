# J2KR
DICOM J2KR compression

Shows how to link openjpeg, kakadu and dcmtk.

Command syntax:

```
xxx
inRelativePathOr*
inBasePath/
doneBasePath
errBasePath
DEBUG_LOG_LEVEL (or INFO_..., or WARN_... or TRACE_...)
```

Notas:
* needs to be completed with libkdu_v73R.a.
* for 8bits and 16bits pixels only (doesn´t work for new float or double pixels)
* dicom.dic needs to be copied to /usr/local/share/dcmtk/dicom.dic as defined in http://support.dcmtk.org/docs/file_datadict.html (DCMDICTPATH environment variable is not read correctly when executing from XCode)
* we used static libraries compiled from 2016-11 version of dcmtk. The headers relative to these static libraries are hierarchical in paths like this dcmtk/module/header.
* dcmtk static libraries are compiled universal 32/64 bit


Project folders:
* dcmtk contains headers, c++ code and static libs of dcmtk
* openjpeg contains headers, c++ code and static libs for newest openjpeg integration
* objective-c wrappers make it easier to use dcmtk C++ classes from objecive-c
* objective-c categories add functionality to objective-c classes
* OD is an objective-c framework (in progress) for dicom file editing
