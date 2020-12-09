import os
from os.path import join, dirname
from dotenv import load_dotenv

dotenv_path = join(dirname(__file__), '.env')
load_dotenv(dotenv_path)

WEBPASS = os.environ.get("SUIKOU_API")  # 環境変数の値をAPに代入
