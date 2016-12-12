# J2KR
DICOM J2KR compression

Executables KDU and OPJ, derived from OsiriX 5.9 and Horos 2.0 for Jpeg 2000 lossless compression of DICOM files. These executables show how to link openjpeg, kakadu and dcmtk.

Command syntax:

```
OPJ | KDU
inRelativePathOr*
inBasePath/
doneBasePath
errBasePath
```

Notas:
* In order to compile the, the KDU target needs to be completed with libkdu_v73R.a.
* for 8bits and 16bits pixels only (doesn´t work for new float or double pixels)
* dicom.dic needs to be copied to /usr/local/share/dcmtk/dicom.dic as defined in http://support.dcmtk.org/docs/file_datadict.html (DCMDICTPATH environment variable is not read correctly when executing from XCode)
