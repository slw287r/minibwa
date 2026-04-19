## Getting Started
```sh
git clone https://github.com/lh3/minibwa
cd minibwa
make        # Or "make omp=0" if the compiler doesn't support OpenMP
./minibwa index test/chrM-human.fa.gz chrM-human              # index the genome
./minibwa map -a chrM-human test/chrM-read_?.fa.gz > aln.sam  # align and output in SAM

# other examples without test data
minibwa map -a -5P ref.index reads.interleaved.fq > aln.sam   # align Hi-C short reads
minibwa map -t16 ref.index long-read.fq > aln.paf             # align long reads
```

## Introduction

Minibwa aligns short reads against a reference genome. It is the successor of
bwa-mem with a different algorithm. Minibwa is over three times as fast as the
original bwa-mem and twice as fast as bwa-mem2 at comparable accuracy. While
minibwa works with accurate long reads, minimap2 is more robust under high
error rate.

Minibwa is a hybrid of bwa-mem and minimap2: it indexes the genome with
Burrow-Wheeler Transform (BWT), finds variable-length seeds like bwa-mem, and
performs chaining and SIMD-based nucleotide alignment with the minimap2
algorithm. Minibwa speeds up bwa-mem2 further with additional prefetch for
seeding, new heuristics to skip unnecessary mate rescue and reduced effort in
highly repetitive regions where reads would often be wrongly mapped due to
structural changes anyway.
