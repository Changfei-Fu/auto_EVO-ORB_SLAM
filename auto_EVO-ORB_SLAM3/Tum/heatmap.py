import matplotlib.pyplot as plt
import numpy as np
import matplotlib as mpl
data=open("./long_data.txt","r")
lines=data.readlines()
image_num=len(lines)
data.close()
print(image_num)
heatmap=np.zeros( (image_num,image_num) )
data=open("./long_data.txt","r")
i=0
for line in data:
    heatmap[i]=line.rstrip().split(' ')
    i+=1

for i in range(0,image_num):
    for j in range(0,image_num):
        if heatmap[i][j]>75:
            print(i)
            heatmap[i][j]=75
print(heatmap.all)


mpl.rcParams["font.size"] = 80
plt.figure(figsize=(50, 40))
plt.imshow(heatmap)

plt.colorbar()
plt.tight_layout()

plt.savefig("./figure.png")
plt.show()



