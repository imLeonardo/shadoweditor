import os
import math
import numpy as np
import tensorflow as tf
import matplotlib.pyplot as plt

# -----------------生成图片路径和标签的List------------------------------------
# 生成图片的存储位置 E:/python-run-env/train-test/Re_train/image_data/inputdata/
train_dir = 'E:/python-run-env/train-test/Re_train/image_data/inputdata/'

cabbage = []
label_cabbage = []
carrot = []
label_carrot = []
nori = []
label_nori = []
potato = []
label_potato = []


# step1：获取'E:/Re_train/image_data/training_image'下所有的图片路径名，存放到
# 对应的列表中，同时贴上标签，存放到label列表中。
# ratio是测试集的比例
def get_files(file_dir, ratio):
    for file in os.listdir(file_dir + '/cabbage'):
        cabbage.append(file_dir + '/cabbage' + '/' + file)
        label_cabbage.append(0)
    for file in os.listdir(file_dir + '/carrot'):
        carrot.append(file_dir + '/carrot' + '/' + file)
        label_carrot.append(1)
    for file in os.listdir(file_dir + '/nori'):
        nori.append(file_dir + '/nori' + '/' + file)
        label_nori.append(2)
    for file in os.listdir(file_dir + '/potato'):
        potato.append(file_dir + '/potato' + '/' + file)
        label_potato.append(3)

    # step2：对生成的图片路径和标签List做打乱处理把所有的数据合起来组成一个list（img和lab）
    # np.hstack水平（按列）按顺序堆叠数组。
    # >>> a = np.array((1,2,3))
    # >>> b = np.array((2,3,4))
    # >>> np.hstack((a,b))
    # array([1, 2, 3, 2, 3, 4])
    image_list = np.hstack((cabbage, carrot, nori, potato))
    label_list = np.hstack((label_cabbage, label_carrot, label_nori, label_potato))

    # 利用shuffle打乱顺序
    temp = np.array([image_list, label_list])
    temp = temp.transpose()
    np.random.shuffle(temp)

    # 从打乱的temp中再取出list（img和lab）
    # image_list = list(temp[:, 0])
    # label_list = list(temp[:, 1])
    # label_list = [int(i) for i in label_list]
    # return image_list, label_list

    # 将所有的img和lab转换成list
    all_image_list = list(temp[:, 0])
    all_label_list = list(temp[:, 1])

    # 将所得List分为两部分，一部分用来训练tra，一部分用来测试val
    # ratio是测试集的比例
    # n_sample全部样本数
    n_sample = len(all_label_list)
    n_val = int(math.ceil(n_sample * ratio))  # 测试样本数
    n_train = n_sample - n_val  # 训练样本数

    # 训练的图片和标签
    tra_images = all_image_list[0:n_train]
    tra_labels = all_label_list[0:n_train]
    tra_labels = [int(float(i)) for i in tra_labels]

    # 测试图片和标签
    val_images = all_image_list[n_train:-1]
    val_labels = all_label_list[n_train:-1]
    val_labels = [int(float(i)) for i in val_labels]

    return tra_images, tra_labels, val_images, val_labels


# --------------------生成Batch----------------------------------------------

# step1：将上面生成的List传入get_batch() ，转换类型，产生一个输入队列queue，因为img和lab
# 是分开的，所以使用tf.train.slice_input_producer()，然后用tf.read_file()从队列中读取图像
#   image_W, image_H, ：设置好固定的图像高度和宽度
#   设置batch_size：每个batch要放多少张图片
#   capacity：一个队列最大多少
def get_batch(image, label, image_W, image_H, batch_size, capacity):
    # 转换类型
    image = tf.cast(image, tf.string)
    label = tf.cast(label, tf.int32)

    # make an input queue
    # tf.train.slice_input_producer是一个tensor生成器，作用是按照设定，
    # 每次从一个tensor列表中按顺序或者随机抽取出一个tensor放入文件名队列。
    input_queue = tf.train.slice_input_producer([image, label])

    label = input_queue[1]
    image_contents = tf.read_file(input_queue[0])  # read img from a queue

    # step2：将图像解码，不同类型的图像不能混在一起，要么只用jpeg，要么只用png等。
    image = tf.image.decode_jpeg(image_contents, channels=3)

    # step3：数据预处理，对图像进行旋转、缩放、裁剪、归一化等操作，让计算出的模型更健壮。
    image = tf.image.resize_image_with_crop_or_pad(image, image_W, image_H)
    image = tf.image.per_image_standardization(image)

    # step4：生成batch
    # image_batch: 4D tensor [batch_size, width, height, 3],dtype=tf.float32
    # label_batch: 1D tensor [batch_size], dtype=tf.int32
    image_batch, label_batch = tf.train.batch([image, label],
                                              batch_size=batch_size,
                                              num_threads=32,
                                              capacity=capacity)
    # 重新排列label，行数为[batch_size]
    label_batch = tf.reshape(label_batch, [batch_size])
    image_batch = tf.cast(image_batch, tf.float32)
    return image_batch, label_batch
