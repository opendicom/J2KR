# J2KR
DICOM J2KR compression

Executables KDU and OPJ, derived from OsiriX 5.9 and Horos 2.0 for Jpeg 2000 lossless compression of DIOCM files. These executables show how to link openjpeg, kakadu and dcmtk.

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
* for 8bits and 16bits pixels 
