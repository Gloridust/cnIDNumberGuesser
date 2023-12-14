
# cnIDNumberGuesser

![GitHub release](https://img.shields.io/github/v/release/Gloridust/cnIDNumberGuesser?style=flat-square)
![GitHub release date](https://img.shields.io/github/release-date/Gloridust/cnIDNumberGuesser?style=flat-square)
![GitHub last commit](https://img.shields.io/github/last-commit/Gloridust/cnIDNumberGuesser?style=flat-square)
![GitHub download latest](https://img.shields.io/github/downloads/Gloridust/cnIDNumberGuesser/latest/total?style=flat-square)
![GitHub download total](https://img.shields.io/github/downloads/Gloridust/cnIDNumberGuesser/total?style=flat-square)  
![GitHub stars](https://img.shields.io/github/stars/Gloridust/cnIDNumberGuesser?style=flat-square)
![GitHub forks](https://img.shields.io/github/forks/Gloridust/cnIDNumberGuesser?style=flat-square)
![GitHub issues](https://img.shields.io/github/issues/Gloridust/cnIDNumberGuesser?style=flat-square)
![GitHub issues closed](https://img.shields.io/github/issues-closed/Gloridust/cnIDNumberGuesser?style=flat-square)  

This document is in English.For Simplified Chinese? [Please click here](readme.md).

## Introduction

This project is designed to guess and complete Chinese mainland ID numbers. It is useful when you don't remember part of your ID number or if a part of the document number is missing. The tool is available in both Python and Web versions. You can choose to run it locally with Python or deploy it on a server and implement it through front-end JS code. Note: This project is intended only for academic research purposes. **Please comply with local laws and do not use it for illegal purposes. The author is not responsible for any misuse or consequences thereof.**

## Deployment and Usage

### Python

1. Download [guessID.py](guessID.py) and [citycodes.txt](citycodes.txt) and place them in the same folder. You can also download the entire repository:

```bash
git clone https://github.com/Gloridust/cnIDNumberGuesser
cd cnIDNumberGuesser
```

2. Install dependencies

```python
pip install psutil
```

Ensure you are using the pip version compatible with your Python. If working in multiple Python environments, you might need to use `pip3` or a specific command like `python -m pip install`.

3. Run the program:

```python
python3 guessID.py
```

4. Usage: Input an 18-digit ID number, replacing missing parts with asterisks "*".

![Python run example](./readme_src/run_py.png)

5. After the program ends, check the results in [resultID.txt](resultID.txt). Please delete the original [resultID.txt](resultID.txt) file before using it again.

![Python result example](./readme_src/result_py.png)

### Web

0. For web deployment, you can [Fork this project](https://github.com/Gloridust/cnIDNumberGuesser/fork), then select a branch and bind a domain on Github Page for immediate use.

![Web page example](./readme_src/page.png)

1. To deploy on your own server, download [index.html](index.html), [script.js](script.js), [style.css](style.css), and [citycodes.txt](citycodes.txt) and place them in the same folder. You can also download the entire repository:

```bash
git clone https://github.com/Gloridust/cnIDNumberGuesser
cd cnIDNumberGuesser
```

2. After deployment, visit [index.html](index.html) to start using. Similarly, input an 18-digit ID number, replacing missing parts with asterisks "*".

![Web run example](./readme_src/run_web.png)

3. Click “GuessID” to run. Once the result is obtained, it will be displayed below. You can also click “Download” to save the result in 'guessedIDs.txt' and download it to your device.

![Web result example](./readme_src/result_web.png)

## Privacy Statement

We fully respect user privacy. Whether it's the Python or Web version, all data is processed locally on the user's end and no data is uploaded.

Similarly, we hope users respect others' privacy and do not use this for illegal purposes; **please comply with local laws and do not use it for illegal purposes. The author is not responsible for any misuse or consequences thereof.**

## Acknowledgements

The Python version of this project and the [citycodes.txt](citycodes.txt) file were inspired by content from [https://github.com/wlkjyh/completion_idcard](https://github.com/wlkjyh/completion_idcard). Many thanks to the author for their open source contribution!

## Open Source License

MIT
