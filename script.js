function validateAndGuessID() {
    const idInput = document.getElementById('idInput').value;
    const resultElement = document.getElementById('result');

    if (!isIDFormatValid(idInput)) {
        resultElement.textContent = '身份证号码格式不正确。';
        return;
    }

    if (isIDValid(idInput)) {
        resultElement.textContent = '有效的身份证号码。';
    } else {
        const guessedIDs = guessID(idInput);
        if (guessedIDs.length > 0) {
            resultElement.textContent = '可能的有效身份证号码: ' + guessedIDs.join(', ');
        } else {
            resultElement.textContent = '未找到有效的身份证号码。';
        }
    }
}

// 检查身份证号码格式（18位，最后一位可能是X，可以包含星号）
function isIDFormatValid(id) {
    const regex = /^[0-9*]{17}[0-9Xx*]$/;
    return regex.test(id);
}

// 验证身份证号码（包括校验码）
function isIDValid(id) {
    if (!isIDFormatValid(id)) return false;
    return checkIDChecksum(id);
}

// 计算并验证校验码
function checkIDChecksum(id) {
    const factors = [7, 9, 10, 5, 8, 4, 2, 1, 6, 3, 7, 9, 10, 5, 8, 4, 2];
    const remainders = ['1', '0', 'X', '9', '8', '7', '6', '5', '4', '3', '2'];
    let sum = 0;

    for (let i = 0; i < 17; i++) {
        sum += parseInt(id[i]) * factors[i];
    }

    const remainder = sum % 11;
    return id[17].toUpperCase() === remainders[remainder];
}

// 猜测身份证号码
function guessID(id) {
    let possibleIDs = [''];
    for (let i = 0; i < id.length; i++) {
        let newPossibleIDs = [];
        if (id[i] === '*') {
            for (let j = 0; j < possibleIDs.length; j++) {
                for (let digit = 0; digit <= 9; digit++) {
                    newPossibleIDs.push(possibleIDs[j] + digit.toString());
                }
            }
        } else {
            for (let j = 0; j < possibleIDs.length; j++) {
                newPossibleIDs.push(possibleIDs[j] + id[i]);
            }
        }
        possibleIDs = newPossibleIDs;
    }

    return possibleIDs.filter(isIDValid);
}

// 下载结果功能
function downloadResults() {
    const resultElement = document.getElementById('result');
    const text = resultElement.textContent;
    const blob = new Blob([text], { type: 'text/plain' });
    const url = URL.createObjectURL(blob);

    const a = document.createElement('a');
    a.href = url;
    a.download = 'guessedIDs.txt';
    document.body.appendChild(a);
    a.click();
    document.body.removeChild(a);
    URL.revokeObjectURL(url);
}
