from flask import Flask
from flask import request
from flask import jsonify
from flask_cors import CORS

app = Flask(__name__)
CORS(app)

# Store data from ESP32
sensor_data = {"switch": "OFF", "light": 0}

# POST endpoint to receive data
@app.route('/post-data', methods=['POST'])
def post_data():
    data = request.get_json()
    if not data:
        return jsonify({"error": "No data received"}), 400

    switch_state = data.get("switch")
    light_sensor_value = data.get("light")

    print(f"Received Switch: {switch_state}, Light Sensor: {light_sensor_value}")
    return jsonify({"message": "Data received successfully"}), 200

@app.route("/get-data", methods=["GET"])
def get_data():
    # Return the latest sensor data
    if sensor_data:
        return jsonify(sensor_data)
    else:
        return jsonify({"error": "No data available"}), 404

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)