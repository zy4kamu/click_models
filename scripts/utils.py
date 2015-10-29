import matplotlib
from pylab import *

def Get_precision(data_file, res_file):
    res_distance = 0
    res = [[], []]
    dists = []
    labels = []
    last_dist = -1
    sum_label = 0
    with open(data_file) as data:
        for line_n, line in enumerate(data):
            if (line_n%10**6 == 0):
                print(line_n)
            line = line.strip().split()
            dist = float(line[-2])
            label = float(line[-1])
            labels.append(label)
            if last_dist == dist:
                sum_label += label
            else:

                if (line_n > 0):
                    dists.append(last_dist)
                    res[0].append(line_n)
                    res[1].append(float(sum_label)/(line_n))
                    if (res[1][-1] > 0.55):
                        res_distance = last_dist
                sum_label += label
            last_dist = dist
    l = len(res[0])
    axis([0., 200, 0.4, 0.6])
    print (dists)
    plot(dists, res[1])
    #plot(labels)
    grid(True)

    savefig(res_file)
    print(res_distance)
    return str(res_distance)

def Get_max(x):
    res = x[0]
    i = 0
    for j in range(len(x)):
        if res < x[j]:
            i = j
            res = x[j]
    return (i, res)

def PlotPrecision(data_file, res_file):
    best_res = {0:7, 1:25, 2:48, 3:29, 4:39, 5:108, 6:179, 7:217, 8:123, 9:3840}
    res = []
    with open(data_file) as data:
        for line_n, line in enumerate(data):
            #if (line_n%10**6 == 0):
            #    print(line_n)
            line = line.strip().split()
            res = [float(i) for i in line[1:]]
            plot(res)
            print Get_max(res),res[-1]
    axis([0,1000,0.2,0.8])
    grid(True)
    savefig(res_file)

def GetCTR(data_file, res_file):
    l  = 10000
    res_labels = [0. for i in range(l)]
    res_n_users = [0. for i in range(l)]
    with open(data_file) as data:
        for line_n, line in enumerate(data):
            if (line_n%10**6 == 0):
                print(line_n)
            line = line.strip().split()[1:]
            line1 = [float(line[i])*(i+1) for i in range(len(line))]
            line[1:] = [line1[i+1] - line1[i] for i in range(len(line1)-1)]
            #print(line)
            #print(line1)
            #line = line1
            if (len(line) > l):
                line = line[:l]
            last_label = 0.
            for i in range(len(line)):
                label  = float(line[i])
                if (label > 0.5):
                    #if (i == 3):
                        #print(line, last_label, label)
                    res_labels[i] += 1
                last_label = label
                res_n_users[i] += 1
    #res_labels = [res_labels[i]/(res_n_users[i]+1e-10) for i in range(10)]
    #res_labels = "\n".join(str(r) for r in res_labels)
    res_labels = "\n".join(str(res_labels[i]/res_n_users[i]) for i in range(l))
    return res_labels

#with open("../../../data_stat/histogramms/dist_271", 'w') as dis:
#    for i in range(100,101,1):
#        print(i)
#        directory = "../../../data_stat/"
#        dis.write(GetCTR(directory + "histogramms/histogramm_" + str(i), directory + "hist_png/histogramm_"
#                                + str(i) + ".png") + " " + str(i) + "\n")
out_dir = "/Users/annasepliaraskaia/Desktop/work/"
#Get_precision(out_dir + "data_stat/histogramms/histogramm_1000", out_dir + "data_stat/histogramms/histogramm_1000.png")
#Get_precision(out_dir + "data_stat/histogramms/histogramm_1001", out_dir + "data_stat/histogramms/histogramm_1001.png")
PlotPrecision(out_dir + "data_stat/histogramms/hist_100", out_dir + "data_stat/histogramms/hist_100.png")

