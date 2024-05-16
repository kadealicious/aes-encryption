<h1>AES-128 Encryption</h1>

<h2>Summary</h2>
This is a C++ implementation of the AES-128 encryption/decryption algorithms, minus the column mixing step.  Using the command line interface, the user can choose to encrypt or decrypt any file; the output data is shown in the terminal as well as being written to a .txt file.

<h2>How to Run</h2>
Using your compiler of choice, compile main.cpp and aes.cpp together.  Example:

```
g++ main.cpp aes.cpp -o aes.exe
```

All that's left to do is create a file for encryption/decryption and run the output executable named "aes.exe"!

<h2>Disclaimer</h2>
We take no responsibility for any problems that arise from your use of the contents of this repo; do not reuse any code found here in production code.
