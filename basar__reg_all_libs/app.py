from flask import Flask, render_template, request, redirect, url_for, flash
import os
from werkzeug.utils import secure_filename

app = Flask(__name__)
app.secret_key = os.environ.get('SESSION_SECRET', 'dev-secret-key')

UPLOAD_FOLDER = 'uploads'
ALLOWED_EXTENSIONS = {'txt', 'pdf', 'png', 'jpg', 'jpeg', 'gif', 'zip', 'tar', 'gz', 'py', 'js', 'json', 'xml', 'csv'}

app.config['UPLOAD_FOLDER'] = UPLOAD_FOLDER
app.config['MAX_CONTENT_LENGTH'] = 16 * 1024 * 1024

os.makedirs(UPLOAD_FOLDER, exist_ok=True)

def allowed_file(filename):
    return '.' in filename and filename.rsplit('.', 1)[1].lower() in ALLOWED_EXTENSIONS

@app.route('/')
def index():
    uploaded_files = []
    if os.path.exists(UPLOAD_FOLDER):
        uploaded_files = os.listdir(UPLOAD_FOLDER)
    return render_template('index.html', files=uploaded_files)

@app.route('/upload', methods=['POST'])
def upload_file():
    if 'file' not in request.files:
        flash('No file part')
        return redirect(request.url)
    
    file = request.files['file']
    
    if file.filename == '' or file.filename is None:
        flash('No selected file')
        return redirect(request.url)
    
    if file and file.filename:
        if not allowed_file(file.filename):
            flash('File type not allowed. Allowed types: txt, pdf, png, jpg, jpeg, gif, zip, tar, gz, py, js, json, xml, csv')
            return redirect(url_for('index'))
        
        filename = secure_filename(file.filename)
        file.save(os.path.join(app.config['UPLOAD_FOLDER'], filename))
        flash(f'File {filename} uploaded successfully!')
        return redirect(url_for('index'))
    
    return redirect(url_for('index'))

@app.route('/analyze/<filename>')
def analyze_file(filename):
    filename = secure_filename(filename)
    
    if not filename:
        flash('Invalid filename')
        return redirect(url_for('index'))
    
    filepath = os.path.join(app.config['UPLOAD_FOLDER'], filename)
    
    upload_folder_abs = os.path.abspath(app.config['UPLOAD_FOLDER'])
    filepath_abs = os.path.abspath(filepath)
    
    if not filepath_abs.startswith(upload_folder_abs + os.sep):
        flash('Invalid file path')
        return redirect(url_for('index'))
    
    if not os.path.exists(filepath):
        flash('File not found')
        return redirect(url_for('index'))
    
    file_info = {
        'name': filename,
        'size': os.path.getsize(filepath),
        'path': filepath
    }
    
    content = None
    try:
        with open(filepath, 'r', encoding='utf-8') as f:
            content = f.read()
    except:
        content = "Binary file or unable to read as text"
    
    return render_template('analyze.html', file_info=file_info, content=content)

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=True)
