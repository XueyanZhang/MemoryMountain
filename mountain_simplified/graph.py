import sys
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

def main():
    # parse csv data
    df = pd.read_csv("data.csv", sep="\t")
    df = df.iloc[:,:-1]

    stride = df.columns.values
    size = df.index.values
    
    # plot 2d graphs
    plt.figure(figsize=(10, 8))
    plt.rcParams.update({"font.size": 16})
    plt.xlabel("Size")
    plt.ylabel("Bandwidth")
    plt.title("Memory Mountain 2d\n")
    plt.plot(size, df.values)
    # plot vertical dashed lines at size label 32k, 256k, 2m, 12m
    plt.axvline(x=np.where(size == '32k')[0][0], color="black", linestyle="--")
    plt.axvline(x=np.where(size == '128k')[0][0], color="blue", linestyle="--")
    plt.axvline(x=np.where(size == '256k')[0][0], color="black", linestyle="--")
    plt.axvline(x=np.where(size == '2m')[0][0], color="black", linestyle="--")
    plt.axvline(x=np.where(size == '4m')[0][0], color="blue", linestyle="--")
    plt.legend(stride, loc="upper left")
    plt.savefig("2D.png", dpi=300)

    # plot 3d graphs
    xticks = stride
    yticks = size
    x = np.arange(len(xticks))  # stride
    y = np.arange(len(yticks))  # size ticks
    x, y = np.meshgrid(x, y)
    z = df.values

    plt.figure(figsize=(10, 8))
    ax = plt.axes(projection="3d")
    ax.set_xticks(range(len(xticks)))
    ax.set_yticks(range(len(yticks)))
    plt.rcParams.update({"font.size": 16})
    ax.set_xticklabels(xticks[:])
    ax.set_yticklabels(yticks[:])
    ax.tick_params(axis="z", pad=10)
    ax.set_xlabel("Stride", fontsize=18, labelpad=20)
    ax.set_ylabel("Size", fontsize=18, labelpad=20)
    ax.set_zlabel("Bandwidth", fontsize=18, labelpad=20)
    ax.set_title("Memory Mountain\n", fontsize=22)
    ax.plot_surface(x, y, z, cmap="jet")
    # https://matplotlib.org/stable/gallery/mplot3d/view_planes_3d.html
    # plt.show()
    plt.savefig("3D.png", dpi=300)
    ax.view_init(0, -90, 0)
    plt.savefig("XZ.png", dpi=300)
    ax.view_init(0, 0, 0)
    plt.savefig("YZ.png", dpi=300)

if __name__ == "__main__":
    main()