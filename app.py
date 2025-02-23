from flask import Flask

app = Flask(__name__)

@app.route('/')
def home():
    return "<h1>Welcome to My Simple Web Page</h1><p>This is a basic Flask web application.</p>"

if __name__ == '__main__':
    app.run(debug=True)
