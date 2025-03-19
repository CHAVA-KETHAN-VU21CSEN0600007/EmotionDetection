import { useState, useEffect } from "react";
import { motion } from "framer-motion";
import { Card } from "./components/Card";
import { Button } from "./components/Button";

const emotionEmojis = {
  happy: "😊",
  sad: "😢",
  angry: "😡",
  surprised: "😲",
  neutral: "😐",
};

export default function EmotionPrediction() {
  const [emotion, setEmotion] = useState(null);
  const [loading, setLoading] = useState(true);

  useEffect(() => {
    fetchEmotion();
  }, []);

  const fetchEmotion = async () => {
    setLoading(true);
    setEmotion(null); // Hide previous emotion while loading
    setTimeout(() => {
      const emotions = Object.keys(emotionEmojis);
      const randomEmotion = emotions[Math.floor(Math.random() * emotions.length)];
      setEmotion(randomEmotion);
      setLoading(false);
    }, 1500);
  };

  return (
    <div className="flex flex-col items-center justify-center min-h-screen bg-gradient-to-br from-blue-100 to-purple-300 p-4">
      <motion.div
        initial={{ opacity: 0, scale: 0.5 }}
        animate={{ opacity: 1, scale: 1 }}
        transition={{ duration: 1 }}
      >
        <Card className="w-full max-w-md p-6 text-center shadow-2xl bg-white rounded-2xl">
          <h1 className="text-3xl font-bold mb-4 text-gray-700">Emotion Prediction</h1>

          {loading ? (
            <p className="text-lg text-gray-500 animate-pulse">Loading...</p>
          ) : (
            <motion.div
              key={emotion}
              initial={{ scale: 0.5, opacity: 0 }}
              animate={{ scale: 1, opacity: 1 }}
              transition={{ duration: 0.5 }}
            >
              <p className="text-5xl mb-4">{emotionEmojis[emotion]}</p>
              <p className="text-lg mb-6 text-gray-600">
                Predicted Emotion: {emotion.charAt(0).toUpperCase() + emotion.slice(1)}
              </p>
            </motion.div>
          )}

          <Button
            onClick={fetchEmotion}
            disabled={loading}
            className="w-full bg-blue-500 hover:bg-blue-600 text-white py-2 rounded-lg transition-all"
          >
            {loading ? "Loading..." : "Refresh Emotion"}
          </Button>
        </Card>
      </motion.div>
    </div>
  );
}
