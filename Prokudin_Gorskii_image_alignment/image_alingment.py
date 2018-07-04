import numpy as np
import matplotlib.pyplot as plt
import cv2 as cv
import os

def load_data(dir_path=None):
    img_name=os.listdir(dir_path)
    img_data=[]
    for i in img_name:
        img=cv.imread(os.path.join(dir_path,i),cv.IMREAD_GRAYSCALE)
        img_data.append(img)
    return img_data

def visualize(image_data,formt=None):
    length=len(image_data)
    #plt.figure(figsize=(20, 40))
    if(length%3==0):
        r=length/3
    else:
        r=(length/3)+1
    
    for i,img in enumerate(image_data):
        subplot_idx=i+1
        plt.subplot(r,3,subplot_idx)
        plt.imshow(img,cmap=formt)
    plt.show()

def canny_edge(image_data,min_value,max_value):
    canny_img=[]
    for img in image_data:
        canny_edge = cv.Canny(img, min_value, max_value,apertureSize = 3)
        canny_img.append(canny_edge)
    return canny_img

def check_border(canny_img):
    y,x=canny_img.shape[0],canny_img.shape[1]
    dy=int(y*0.1)
    dx=int(x*0.1)
    max_xbrow=0
    max_xurow=0
    max_ylrow=0
    max_yrrow=0
    count=[]
    for j in range(dy):
        counts=0
        for i in range(x):
            if canny_img[i,x-j-1]==255:
                counts+=1
        count.append(counts)
        
    idx1=count.index(max(count))
    count.remove(max(count))
    idx2=count.index(max(count))
    max_xbrow=max(idx1,idx2)
    count=[]
    for j in range(dx):
        counts=0
        for i in range(y):
            if canny_img[i,x-j-1]==255:
                counts+=1
        count.append(counts)

    idx1=count.index(max(count))
    count.remove(max(count))
    idx2=count.index(max(count))
    max_ylrow=max(idx1,idx2)
    count=[]

    for j in range(dy):
        counts=0
        for i in range(x):
            if canny_img[i,x-j-1]==255:
                counts+=1
        count.append(counts)

    idx1=count.index(max(count))
    count.remove(max(count))
    idx2=count.index(max(count))
    max_xurow=max(idx1,idx2)
    count=[]
    
    for j in range(dx):
        counts=0
        for i in range(y):
            if canny_img[i,x-j-1]==255:
                counts+=1
        count.append(counts)

    idx1=count.index(max(count))
    count.remove(max(count))
    idx2=count.index(max(count))
    max_yrrow=max(idx1,idx2)
    
    return max_xbrow,max_xurow,max_ylrow,max_yrrow,y,x

def remove_border(img_data):
    low_threshold=65
    up_threshold=130
    canny_img=canny_edge(img_data,low_threshold,up_threshold)
    visualize(canny_img,'gray')
    image_border_removed=[]
    for i,img in enumerate(canny_img):
        max_xbrow,max_xurow,max_ylrow,max_yrrow,y,x=check_border(img)
        img_d=img_data[i]
        img_br=img_d[max_xbrow:y-max_xurow-1,max_ylrow:x-max_yrrow-1]
        image_border_removed.append(img_br)
    return image_border_removed

def image_combine(img_channel):
    l,b=img_channel.shape
    r=l%3
    s=int(l/3)
    lb=0
    ub=s
    rgb=np.zeros(((s+r),b,3))
    rgb[:s,:,2]+=img_channel[lb:ub,:]
    lb=ub
    ub+=s
    rgb[:s,:,1]+=img_channel[lb:ub,:]
    lb=ub
    ub+=s
    rgb[:s+r,:,0]+=img_channel[lb:ub+r,:]
    rgb=rgb.astype('uint8')
    return rgb
               

def image_rgb(img):
    img_rgb=[]
    for i in img:
        rgb=image_combine(i)
        img_rgb.append(rgb)
    return img_rgb

def sdd(img):

    l=img.shape[0]
    b=img.shape[1]
    Z=np.zeros((l,b))
    img_channel=[]
    img_channel.append(img[:,:,0])
    img_channel.append(img[:,:,1])
    img_channel.append(img[:,:,2])
    img_shiftg=img_channel[1].copy()
    img_shiftb=img_channel[2].copy()
    hshift=0
    vshift=0
    sums=np.inf
    n=1/(l*b)
    for i in range(-15,16):
        for j in range(-15,16):
            Z=np.zeros((l,b))
            if i>0 and j>0:
                Z[0:l-j,0:b-i]+=(img_channel[1][j:,i:]).copy()
                s=(((img_channel[0]-Z)**2)*n).sum()
            elif i<0 and j<0:
                Z[0-j+1:,0-i+1:]+=(img_channel[1][:l+j-1,:b+i-1]).copy()
                s=(((img_channel[0]-Z)**2)*n).sum()
            elif i>0 and j<0:
                Z[0-j+1:,:b-i]+=(img_channel[1][:l+j-1,i:]).copy()
                s=(((img_channel[0]-Z)**2)*n).sum()
            elif i<0 and j>0:
                Z[:l-j,0-i+1:]+=(img_channel[1][j:,:b+i-1]).copy()
                s=(((img_channel[0]-Z)**2)*n).sum()
            if s<sums:
                sums=s
                img_shiftg=Z

    sums=np.inf           
    for i in range(-15,16):
        for j in range(-15,16):
            Z=np.zeros((l,b))
            if i>0 and j>0:
                Z[0:l-j,0:b-i]+=(img_channel[2][j:,i:]).copy()
                s=(((img_channel[0]-Z)**2)*n).sum()
            elif i<0 and j<0:
                Z[0-j+1:,0-i+1:]+=(img_channel[2][:l+j-1,:b+i-1]).copy()
                s=(((img_channel[0]-Z)**2)*n).sum()
            elif i>0 and j<0:
                Z[0-j+1:,:b-i]+=(img_channel[2][:l+j-1,i:]).copy()
                s=(((img_channel[0]-Z)**2)*n).sum()
            elif i<0 and j>0:
                Z[:l-j,0-i+1:]+=(img_channel[2][j:,:b+i-1]).copy()
                s=(((img_channel[0]-Z)**2)*n).sum()
            if s<sums:
                sums=s
                img_shiftb=Z
    
    img[:,:,1]=img_shiftg
    img[:,:,2]=img_shiftb
    img=img.astype('uint8')
    return img
            

def image_align(img,typ='SDD'):
    img_align=[]
    if typ=='SDD':
        for i in img:
            i_align=sdd(i)
            img_align.append(i_align)

    return img_align

dir_path=r'C:\Users\uma\Desktop\Python_exp\plates' #give path where Prokudin-Gorkskii plates are present
img_data=load_data(dir_path) #load images
visualize(img_data,'gray') #visualize images
img_border_removed=remove_border(img_data) #remove border
visualize(img_border_removed,'gray')


img_rgb=image_rgb(img_border_removed) #combine RGB channel
visualize(img_rgb)

img_align=image_align(img_rgb,'SDD') #align RGB channel
visualize(img_align)




