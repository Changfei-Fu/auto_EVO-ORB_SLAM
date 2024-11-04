import matplotlib.pyplot as plt
import numpy as np
import matplotlib.font_manager as fm
import matplotlib as mpl
data=open("./mid_data3.txt","r")
lines=data.readlines()
image_num=len(lines)
data.close()
print(image_num)
pointline=np.zeros(image_num)
final_pointline=np.zeros(int(image_num/3+1))
x=np.zeros(image_num)
final_x=np.zeros(int(image_num/3+1))
data=open("./mid_data3.txt","r")
i=0
for line in data:
    pointline[i]=int(line.rstrip())
    i+=1

for i in range(0,image_num):
    if(i>0):
        x[i]=i+1
        pointline[i]+=pointline[i-1]
j=0
k=1
x[0]=0
final_x[0]=0
final_pointline[0]=0
for i in range(0,image_num):
    j+=1
    if(j==3):
        j=0
        final_x[k]=x[i]
        final_pointline[k]=pointline[i]
        k+=1

print(final_pointline)
print(final_x)
data_sum=open("./sum.txt","a")
for i in range(0,len(pointline)):
    print(pointline[i],file=data_sum)
data_sum.close()
mpl.rcParams["font.size"] =60
plt.figure(figsize=(30,30))
plt.plot(final_x,final_pointline,linewidth=4,color='blue',marker="*",markersize=32)
plt.xlabel("Size of Matched Images")
plt.ylabel("Accumulated Feature Matches")
plt.xticks(range(0,31,3))
#plt.title("Mid-term Data Association")
plt.legend()
plt.tight_layout()
plt.savefig("pointline3.png")
plt.show()




