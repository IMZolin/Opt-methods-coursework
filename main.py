from math import exp
import random
import numpy as np
from random import sample




def save(data,path):
    mat = np.matrix(data)
    with open(path, 'wb') as f:
        for line in mat:
            np.savetxt(f, line, fmt='%i')

def read(file):
    data = np.loadtxt(file, dtype='i', delimiter=' ')
    return data


def DecreaseTemperature(initialTemp, i):
    T = initialTemp*0.1/i
    return T

def GetProbability(R1,R2, T):
    P = exp(-(R1-R2)/T)
    return P

def IsTransition(probability):
    value = random.seed()

    if (value <= probability):
        return True
    else: return False

def generate_data(n,rmin=10,rmax=70):
    data = np.random.randint(0,100,(n,n))
    save(data,'output.txt')
    rating = sample(range(rmin, rmax), n)
    save(rating,'rating.txt')
    return data, rating

def read_data_and_rating():
    data = read('output.txt')
    rating = read('rating.txt')
    return data, rating


def calculate_rating(data, rating, path, max_time):
    current_time = 0
    current_rating =0
    result_time = 0
    counter = 0
    for el in range(1,len(path)):
        counter+=1
        prev_el = el-1
        result_time = current_time
        current_time += data[path[el], path[prev_el]]
        # print(f'время между {path[prev_el]} и {path[el]} = {data[path[el], path[prev_el]]} ')
        if (current_time < max_time):
            current_rating+=(rating[path[prev_el]])
            # print(f'Рейтинг при добавлении {path[prev_el]}-ого города = {current_rating}')
        else:
            # print(f'Рейтинг при добавлении {path[prev_el]}-ого города = {current_rating}')
            #
            # print(f'Суммарное время = {result_time}')
            current_rating += (rating[path[prev_el]])
            return current_rating,result_time, el
        # print(f'Суммарное время = {current_time}')
    current_rating += rating[path[counter]]
    # print(f'Суммарное время = {result_time}')
    # print(f'Суммарный рейтинг = {current_rating}')
    return current_rating,result_time, counter

def calculate_index(n):
    i, j = 0, 0
    while (i == j):
        i = random.randint(0, n-1)
        j = random.randint(0, n-1)
    return i, j

def increase_temp(T_start):
    return T_start*0.5

def swap(path,n):
    i, j = calculate_index(n)
    path[i], path[j] = path[j], path[i]
    return path

def algoritm(n, T_start,T_end,max_time):
    data,rating = read_data_and_rating()
    start_path = sample(range(0, n), n)
    temp_path,max_rating,current_rating,current_time,count,temp_count = 0,0,0,0,0,0
    # print(start_path)
    while (T_start > T_end):
        # max_rating = calculate_rating(data,rating,start_path,max_time)
        # T_start = increase_temp(T_start)
        # start_path = swap(start_path,n)
        # print(f'Номер итерации = {iter}')
        temp_count = count
        temp_rating = current_rating
        current_rating,current_time,count = calculate_rating(data, rating,start_path,max_time)
        T_start = increase_temp(T_start)
        if (current_rating<max_rating):
            probability = GetProbability(max_rating,current_rating,T_start)
            rand_probability = random.uniform(0,1)
            if (probability<rand_probability):
                # print(f'Rand_prob = {rand_probability}, prob = {probability}')
                # print(f'Максимальный рейтинг {max_rating}')
                # print(f'Текущий рейтинг {current_rating}')
                # print(f'Количество пройденных городов: {count}')
                # print(f'Текущее время {current_time}')
                return temp_path, max_rating,temp_count
            current_rating = temp_rating
            # print(f'Rand_prob = {rand_probability}, prob = {probability}')
            # print('хуй')
        elif ((current_rating >= max_rating) and (T_start > T_end)):
            max_rating = current_rating
            temp_path = start_path.copy()
            start_path = swap(start_path, n)
        # print(f'Максимальный рейтинг {max_rating}')
        # print(f'Текущий рейтинг {current_rating}')
        # print(f'Количество пройденных городов: {count}')
        # print(f'Текущее время {current_time}')
        # print(start_path)
    return temp_path, max_rating, temp_count


def main(n):
    max_rate = 0
    best_path = 0
    res_count = 0
    data, rate = read_data_and_rating()
    for i in range(50):
        path,rating, count = algoritm(n,1000,0.1,150)
        if (rating>max_rate):
            max_rate = rating
            best_path = path
            res_count = count

        # print(path)
        # print(count)
    print(best_path)
    print(f'Пройденный путь {best_path[:res_count]}')
    print(f'Рейтинг {max_rate}')
    print(f'Суммарный рейтинг всех городо {sum(rate)}')

main(10)


