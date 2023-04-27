#pragma once

namespace nitro {
    class ProgressListener {
    public:
        virtual ~ProgressListener() = default;

        virtual void progressUpdated(float value) = 0;
    };

}  // namespace nitro
