## Software Implementation of the Post-Quantum Algorithm PERK
Implementations of the PERK algorithm optimized for ARMv8.5-A architecture (Apple M1 processor) for my bachelor's thesis

For compiling, use `make VERSION=<FAST,SHORT><1,3,5><3,5>` to select version, example: `make VERSION=FAST13`
Then use `./main` for testing with KATs or `sudo ./bench` to run the benchmark

The written thesis can be found [here](https://www.ic.unicamp.br/~reltech/PFG/2023/PFG-23-42.pdf) (Portuguese only)

### Performance Results

| Parameter Set | Keygen | Sign | Verify |
| --- | --- | --- | ---|
| PERK-I-fast3 | 54k | 12.1M | 5.9M |
| PERK-I-fast5 | 65k | 11.9M | 5.7M |
| PERK-I-short3 | 54k | 64.8M | 31.6M |
| PERK-I-short5 | 65k | 61.3M | 29.4M |
| PERK-III-fast3 | 120k | 28.9M | 14.3M |
| PERK-III-fast5 | 136k | 28.0M | 13.7M |
| PERK-III-short3 | 120k | 156M | 77.3M |
| PERK-III-short5 | 142k | 147M | 71.5M |
| PERK-V-fast3 | 198k | 59.0M | 30.3M |
| PERK-V-fast5 | 220k | 57.0M | 29.1M |
| PERK-V-short3 | 199k | 319M | 164M |
| PERK-V-short5 | 221k | 296M | 151M |

Performance results were obtained with a MacBook Air using an Apple M1 processor of 3.2GHz
