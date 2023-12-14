from flask import Flask, request

app = Flask(__name__)

@app.route('/process_id', methods=['POST'])
def process_id():
    id_number = request.form['idNumber']
    # 这里调用修改后的 guessID.py 脚本处理 id_number
    # 并返回结果
    result = "处理结果"  # 假设结果
    return result

if __name__ == '__main__':
    app.run(debug=True)
