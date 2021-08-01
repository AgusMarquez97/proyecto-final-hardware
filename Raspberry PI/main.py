
from http.server import BaseHTTPRequestHandler, HTTPServer
import logging
import json
import requests #https://realpython.com/api-integration-in-python/

class RaspberryServer(BaseHTTPRequestHandler):
    def _set_response(self):
        self.send_response(200)
        self.send_header('Content-type', 'text/html')
        self.end_headers()

    def do_POST(self):
        content_length = int(self.headers['Content-Length']) # <--- Gets the size of data
        post_data = self.rfile.read(content_length).decode('utf-8') # <--- Gets the data itself
        logging.info(" POST con request el siguiente req body:\n%s\n", post_data)
        postObject = json.loads(post_data)
        logging.info(" objeto pareseado: %s", postObject)

        self._set_response()

def run(server_class=HTTPServer, handler_class=RaspberryServer, serverPort=8080):
    logging.basicConfig(level=logging.INFO,)
    server_address = ('', serverPort)
    httpd = server_class(server_address, handler_class)
    logging.info(" Server started http://localhost:%s", serverPort)
    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        pass
    httpd.server_close()
    logging.info(' Stopping httpd...\n')

if __name__ == '__main__':
    from sys import argv

    if len(argv) == 2:
        run(port=int(argv[1]))
    else:
        run()
    