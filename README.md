# 1024 Bit Hashing

This algorithm proudces a 1024 bit aimed at user authentication and therefore has a fixed deterministic salt. I have tried to make it more or less quantum resistant from whetever reasearch I have done.

## Why Quantum Resistant?
This algorithm offers a level of quantum resistance by incorporating several design choices that increase the complexity of reverse-engineering and brute-force attacks, even in the face of quantum computing advancements. Algorithms like Grover's algorithm, which reduce brute-force search times, are less effective against the high memory usage and multiple rounds of mixing applied here and also due to it being 1024 bits as 2^512 operations are required which is still very high. GPU acceleration will not be much effective till a extent due to 5mb of memory hardening


