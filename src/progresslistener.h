#ifndef PROGRESSLISTENER_H
#define PROGRESSLISTENER_H

class ProgressListener {
 public:
  virtual ~ProgressListener() {}
  virtual void progressUpdated(int value) = 0;
};

#endif  // PROGRESSLISTENER_H
