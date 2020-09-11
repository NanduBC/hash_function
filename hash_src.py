import sys
import time

filename = sys.argv[1]
ca_rules = [30, 90, 150, 30, 210, 30, 90, 150]
BITRATE = 92
CAPACITY = 132
HASHLEN = 256


def xor(statebits, block):
    for pos, bit in enumerate(block):
        statebits[pos] ^= int(bit)
    print('Statebits: ', statebits)


def omflip_permute(statebits):
    pass


def calculate_3CA(s_minus_one, s, s_plus_one, rule_number):
    if rule_number == 30:
        return s_minus_one ^ s ^ s_plus_one ^ (s & s_plus_one)
    elif rule_number == 90:
        return s_minus_one ^ s_plus_one
    elif rule_number == 150:
        return s_minus_one ^ s ^ s_plus_one
    elif rule_number == 210:
        return s_minus_one ^ s_plus_one ^ (s & s_plus_one)


def apply_ca(statebits):
    '''
    1-null 3 neighborhood CA
    '''
    temp = []
    for i in range(len(statebits)):
        if i == 0:
            next_statebits = calculate_3CA(
                1,
                statebits[i],
                statebits[i+1],
                ca_rules[i % 8])
        elif i == len(statebits)-1:
            next_statebits = calculate_3CA(
                statebits[i-1],
                statebits[i],
                0,
                ca_rules[i % 8])
        else:
            next_statebits = calculate_3CA(
                statebits[i-1],
                statebits[i],
                statebits[i+1],
                ca_rules[i % 8])
        temp.append(next_statebits)
    statebits = temp


def apply_sponge(statebits):
    omflip_permute(statebits)
    apply_ca(statebits)


def calculate_hash(filename):
    last_block = None
    statebits = [0]*(BITRATE + CAPACITY)
    abs_start_time = time.time()
    # Absorption phase
    with open(filename, 'r') as message_file:
        block = message_file.read(BITRATE)
        while block:
            if len(block) < BITRATE:
                last_block = block
                break
            else:
                xor(statebits, block)
                apply_sponge(statebits)
                block = message_file.read(BITRATE)
        if last_block:
            last_block = last_block + '1'\
                + '0'*(BITRATE - len(last_block) - 1)
        xor(statebits, last_block)
        apply_sponge(statebits)
    abs_end_time = time.time()

    # To prevent sliding window attack
    apply_sponge(statebits)

    # Squeezing phase
    hash_value = ''
    sqz_start_time = time.time()
    while(len(hash_value) < HASHLEN):
        apply_sponge(statebits)
        hash_value += ''.join(list(map(str, statebits[:BITRATE])))
    sqz_end_time = time.time()
    print('Absorption phase time: ', abs_end_time-abs_start_time)
    print('Squeezing phase time: ', sqz_end_time-sqz_start_time)
    return hash_value[:HASHLEN]


print(calculate_hash(filename))
