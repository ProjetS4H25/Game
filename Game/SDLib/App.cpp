#pragma once

class App {
private:

public:
  App() {
    
  }

  virtual void handleEvent() = 0;
  virtual void handleUpdate(double deltaTime) = 0;
  virtual void handleDraw() = 0;
};