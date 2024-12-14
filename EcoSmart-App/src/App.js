import React, { useState, useEffect } from 'react';
import { LightningBoltIcon, RocketIcon } from '@radix-ui/react-icons';
import './App.css';

function App() {
  const [data, setData] = useState(null);

  // Fetch data from the Flask server
  useEffect(() => {
    const fetchData = async () => {
      try {
    
        const response = await fetch('http://54.215.158.62:5000/get-data');
        if (!response.ok) {
          throw new Error(`HTTP error! status: ${response.status}`);
        }
        const result = await response.json();
        setData(result);
      } catch (error) {
        console.error('Error fetching data:', error);
      }
    };

    fetchData();
  }, []);


  return (
    <div className="App min-h-screen bg-gray-100 text-gray-800 flex flex-col items-center justify-center p-6">
      {/* Header Section */}
      <header className="flex items-center space-x-4 mb-12">
        <LightningBoltIcon className="w-12 h-12 text-green-500" />
        <h1 className="text-4xl font-extrabold text-gray-900">EcoSmart</h1>
        <LightningBoltIcon className="w-12 h-12 text-green-500" />
      </header>

      {/* Content Section */}
      <div className="bg-white shadow-md rounded-lg p-8 max-w-md text-center">
        <RocketIcon className="w-8 h-8 text-blue-500 mx-auto mb-4" />
        <h2 className="text-2xl font-semibold mb-2">Message from ESP32</h2>
        {data ? (
          <div className="mt-4">
            <p>Switch: <span className="font-semibold">{data.switch}</span></p>
            <p>Light Sensor: <span className="font-semibold">{data.light}</span></p>
          </div>
        ) : (
          <p className="text-gray-500 mt-4">Fetching data...</p>
        )}
      </div>

      {/* Footer Section */}
      <footer className="mt-12 text-sm text-gray-500">
        &copy; {new Date().getFullYear()} EcoSmart, All Rights Reserved.
      </footer>
    </div>
  );
}

export default App;
