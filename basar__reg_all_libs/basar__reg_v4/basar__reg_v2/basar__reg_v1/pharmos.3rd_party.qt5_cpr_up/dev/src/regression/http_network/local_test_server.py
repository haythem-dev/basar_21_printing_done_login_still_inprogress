#!/usr/bin/env python3
"""
Simple HTTP test server for Qt network testing on AIX
Provides basic HTTP endpoints for GET, POST, PUT testing
"""

import json
import socketserver
from http.server import HTTPServer, BaseHTTPRequestHandler
import urllib.parse
from datetime import datetime

class TestHTTPRequestHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        """Handle GET requests"""
        self.send_response(200)
        self.send_header('Content-type', 'application/json')
        self.send_header('Access-Control-Allow-Origin', '*')
        self.end_headers()
        
        response = {
            "method": "GET",
            "path": self.path,
            "headers": dict(self.headers),
            "timestamp": datetime.now().isoformat(),
            "server": "Qt AIX Test Server",
            "status": "success"
        }
        
        self.wfile.write(json.dumps(response, indent=2).encode('utf-8'))
    
    def do_POST(self):
        """Handle POST requests"""
        content_length = int(self.headers.get('Content-Length', 0))
        post_data = self.rfile.read(content_length)
        
        self.send_response(200)
        self.send_header('Content-type', 'application/json')
        self.send_header('Access-Control-Allow-Origin', '*')
        self.end_headers()
        
        try:
            data = json.loads(post_data.decode('utf-8'))
        except:
            data = post_data.decode('utf-8')
        
        response = {
            "method": "POST",
            "path": self.path,
            "headers": dict(self.headers),
            "data": data,
            "timestamp": datetime.now().isoformat(),
            "server": "Qt AIX Test Server",
            "status": "success"
        }
        
        self.wfile.write(json.dumps(response, indent=2).encode('utf-8'))
    
    def do_PUT(self):
        """Handle PUT requests"""
        content_length = int(self.headers.get('Content-Length', 0))
        put_data = self.rfile.read(content_length)
        
        self.send_response(200)
        self.send_header('Content-type', 'application/json')
        self.send_header('Access-Control-Allow-Origin', '*')
        self.end_headers()
        
        try:
            data = json.loads(put_data.decode('utf-8'))
        except:
            data = put_data.decode('utf-8')
        
        response = {
            "method": "PUT",
            "path": self.path,
            "headers": dict(self.headers),
            "data": data,
            "timestamp": datetime.now().isoformat(),
            "server": "Qt AIX Test Server",
            "status": "success"
        }
        
        self.wfile.write(json.dumps(response, indent=2).encode('utf-8'))
    
    def log_message(self, format, *args):
        """Log HTTP requests"""
        print(f"[{datetime.now().strftime('%Y-%m-%d %H:%M:%S')}] {format % args}")

def run_server(host='localhost', port=8080):
    """Run the test HTTP server"""
    server_address = (host, port)
    httpd = HTTPServer(server_address, TestHTTPRequestHandler)
    
    print(f"Qt AIX Test HTTP Server starting on {host}:{port}")
    print(f"Test endpoints available:")
    print(f"  GET  http://{host}:{port}/get")
    print(f"  POST http://{host}:{port}/post")
    print(f"  PUT  http://{host}:{port}/put")
    print(f"Press Ctrl+C to stop server")
    
    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        print("\nShutting down server...")
        httpd.shutdown()
        httpd.server_close()

if __name__ == '__main__':
    import sys
    
    host = 'localhost'
    port = 8080
    
    if len(sys.argv) > 1:
        port = int(sys.argv[1])
    if len(sys.argv) > 2:
        host = sys.argv[2]
    
    run_server(host, port)