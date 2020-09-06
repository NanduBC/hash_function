import sys

filename = sys.argv[1]
ca_rules = [30, 90, 150, 30, 210, 30, 90, 150]


def calculate_3CA(s_minus_one, s, s_plus_one, rule_number):
    if rule_number == 30:
        return s_minus_one ^ s ^ s_plus_one ^ (s & s_plus_one)
    elif rule_number == 90:
        return s_minus_one ^ s_plus_one
    elif rule_number == 150:
        return s_minus_one ^ s ^ s_plus_one
    elif rule_number == 210:
        return s_minus_one ^ s_plus_one ^ (s & s_plus_one)


def apply_ca(state):
    temp = []
    for i in range(len(state)):
        if i == 0:
            next_state = calculate_3CA(
                1,
                state[i],
                state[i+1],
                ca_rules[i % 8])
        elif i == len(state)-1:
            next_state = calculate_3CA(
                state[i-1],
                state[i],
                0,
                ca_rules[i % 8])
        else:
            next_state = calculate_3CA(
                state[i-1],
                state[i],
                state[i+1],
                ca_rules[i % 8])
        temp.append(next_state)
    state = temp


def calculate_hash(filename):
    with open(filename, 'r') as message_file:
        print(message_file.read(12))

calculate_hash(filename)
