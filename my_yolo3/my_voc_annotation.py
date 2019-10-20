import xml.etree.ElementTree as ET
from os import getcwd

sets=['train', 'val', 'test']

classes = ["tomato"]

def convert_annotation(img_id, list_file):
    in_file = open('VOC2007\Annotations\%s.xml' %img_id, encoding='utf-8')
    tree=ET.parse(in_file)
    root = tree.getroot()

    for obj in root.iter('object'):
        difficult = obj.find('difficult').text
        cls = obj.find('name').text
        if cls not in classes or int(difficult)==1:
            continue
        cls_id = classes.index(cls)
        xmlbox = obj.find('bndbox')
        b = (int(xmlbox.find('xmin').text), int(xmlbox.find('ymin').text), int(xmlbox.find('xmax').text), int(xmlbox.find('ymax').text))
        list_file.write(" " + ",".join([str(a) for a in b]) + ',' + str(cls_id))

for image_set in sets:
    img_names = open('VOC2007\ImageSets\Main\%s.txt'%image_set).read().strip().split()     
    list_file = open('2007_%s.txt'%image_set, 'w')
    for img_name in img_names:
        list_file.write('VOC2007\JPEGImages\%s.jpg'%img_name)
        img_id = img_name.split('.')
        convert_annotation(img_id[0], list_file)
        list_file.write('\n')
    list_file.close()

