#pragma once

#include "common/Collection.hpp"  // Include the ICollection interface header file
#include "common/Observable.hpp"  // Include the ICollection interface header file
#include "visitors/Visitable.hpp" // Include the IVisitable interface header file

#include <stdint.h>

class ISensorsCalculator;
class KnobMotionVector;
class HIDEventBuffer;
class IPrinterVisitor;

/// @brief Number of motionvectors that can be added to the collection.
/// @details This is a constant value that defines the maximum number of motionvectors that can be added to the collection.
constexpr uint8_t c_KNOB_MAX_MOTIONVECTORS = 6;

/// @brief Number of observers that can be added to this collection.
/// @details This is a constant value that defines the maximum number of observers that can be added to the collection.
constexpr uint8_t c_KNOB_MAX_MOTIONVECTORCOLLECTION_OBSERVERS = 4;

/**
 * @brief Class representing a visitable,observable collection of motionvectors for the SpaceMouse Knob
 * @details The class implements the ICollection interface and provides functionality for managing a collection of motionvectors.
 *          The class implements the Observable interface, allowing it to notify observers of changes in the collection.
 *         The class implements the Visitable interface, allowing it to accept visitors.
 * @note The KnobMotionVectorCollection class is designed to manage a fixed number of motionvectors and their associated observers.
 */
class KnobMotionVectorCollection : public Collection, public Observable, public Visitable {
private:
public:
    /// @brief Constructor for empty KnobMotionVectorCollection
    KnobMotionVectorCollection() : Collection(c_KNOB_MAX_MOTIONVECTORS), Observable(c_KNOB_MAX_MOTIONVECTORCOLLECTION_OBSERVERS) {}
    ~KnobMotionVectorCollection() {
    }

    /**
     * @brief Set up the knob motionvector collection based on the configuration.
     * @details This function initializes the knob motionvector collection and sets up the motionvectors based on the configuration.
     *          It creates instances of the motionvectors and configures them according to the provided configuration.
     */
    void setup(ISensorsCalculator *sensorsCalculator);
    void setup(ISensorsCalculator *sensorsCalculator, IObserver *hidEventBufferTranslation, IObserver *hidEventBufferRotation);

    KnobMotionVector *getMotionVector(const uint8_t id) const;
    KnobMotionVector *getMotionVector(const char *descriptor) const;

    /**
     * @brief Evaluate all items in the collection and notify observers of the changes.
     * @details This function iterates through all items in the collection and calls their evaluate method.
     *          This allows each item to perform its own evaluation and update its state accordingly.
     * @note Derived classes are allowed to override this method to provide custom evaluation logic.
     */
    void evaluate() override {
        Collection::evaluate();        // Evaluate all axes in the collection
        Observable::notifyObservers(); // Notify observers of changes in the axis collection
    };

    /**
     * @brief Attach an observer to all motionvectors in the collection.
     */
    void attachAxesObserver(IObserver *observer);

    /**
     * @brief Accept the Printer Visitor for all motionvectors in the collection.
     */
    void acceptAxesVisitor(IPrinterVisitor &printerVisitor);
};
