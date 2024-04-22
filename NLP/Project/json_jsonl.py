import json

def json_to_jsonl(input_file, output_file):
    with open(input_file, 'r') as json_file:
        data = json.load(json_file)
        
    with open(output_file, 'w') as jsonl_file:
        for item in data:
            json.dump(item, jsonl_file)
            jsonl_file.write('\n')

input_file = 'output_file.json'
output_file = 'output.jsonl'
json_to_jsonl(input_file, output_file)
