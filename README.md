# J2KR
DICOM J2KR compression

The executables KDU and OPJ show how to link openjpeg, kakadu and dcmtk. We learned from Lance Physer and Aaron Boxer works used in OsiriX and Horos, and are pushing it a bit further.

Command syntax:

```
OPJ | KDU
inRelativePathOr*
inBasePath/
doneBasePath
errBasePath
DEBUG_LOG_LEVEL (or INFO, or WARN or TRACE)
```

Notas:
* in order to compile the, the KDU target needs to be completed with libkdu_v73R.a.
* for 8bits and 16bits pixels only (doesn´t work for new float or double pixels)
* dicom.dic needs to be copied to /usr/local/share/dcmtk/dicom.dic as defined in http://support.dcmtk.org/docs/file_datadict.html (DCMDICTPATH environment variable is not read correctly when executing from XCode)
* we used static libraries compiled from 2016-11 version of dcmtk for everything except the dcmjpeg module. The headers relative to these static libraries are hierarchical in paths like this dcmtk/module/header.
* we compile dcmjpeg within J2KR and removed the headers of this module from the hierarchy.
