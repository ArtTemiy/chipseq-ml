import pandas as pd


def read_bed(file):
    return pd.read_csv(file, sep='\t', header=None)
