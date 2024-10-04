import matplotlib.pyplot as plt
import numpy
import argparse

parser = argparse.ArgumentParser(description='Approximate smthg')
parser.add_argument('n', type=int, default=10, help='number of interpolation nodes')
args = parser.parse_args()
print(args.n)
