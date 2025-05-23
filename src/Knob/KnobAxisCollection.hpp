#pragma once

#include <base/collection/AxisCollectionBase.hpp> // Base class for the collection of Axis objects
#include <common/Observable.hpp>                  // Include the ICollection interface header file
#include <base/visitable/VisitableBase.hpp>       // Include the IVisitable interface header file

#include <stdint.h>

class ISensorsCalculator;
class KnobAxis;
class HIDEventBuffer;

/// @brief Number of axes that can be added to the collection.
/// @details This is a constant value that defines the maximum number of axes that can be added to the collection.
constexpr uint8_t c_KNOB_MAX_AXES = 6;

/// @brief Number of observers that can be added to this collection.
/// @details This is a constant value that defines the maximum number of observers that can be added to the collection.
constexpr uint8_t c_KNOB_MAX_AXISCOLLECTION_OBSERVERS = 4;

/**
 * @brief Class representing a visitable,observable collection of Axis objects for the SpaceMouse Knob
 * @details The class implements the ICollection interface and provides functionality for managing a collection of axes.
 *          The class implements the Observable interface, allowing it to notify observers of changes in the collection.
 *          The class implements the VisitableBase interface, allowing it to accept visitors.
 * @note The KnobAxisCollection class is designed to manage a fixed number of axes and their associated observers.
 */
class KnobAxisCollection : public AxisCollectionBase, public Observable, public VisitableBase {
private:
public:
    KnobAxisCollection()
        : AxisCollectionBase(c_KNOB_MAX_AXES),
          Observable(c_KNOB_MAX_AXISCOLLECTION_OBSERVERS) {}
    ~KnobAxisCollection() {}

    /**
     * @brief Set up the KnobAxisCollection based on the configuration.
     * @details This function initializes the KnobAxisCollection and instantiates the axes based on the configuration.
     *          It creates instances of the axes and configures them according to the provided configuration.
     */
    void setup(ISensorsCalculator *sensorsCalculator);
    void setup(ISensorsCalculator *sensorsCalculator, IObserver *hidEventBufferTranslation, IObserver *hidEventBufferRotation);

    KnobAxis *getAxis(const uint8_t id) const;
    KnobAxis *getAxis(const char *descriptor) const;
    KnobAxis *getAxis(const MotionVector_t type) const;

    /**
     * @brief Evaluate all items in the collection and notify observers of the changes.
     * @details This function iterates through all items in the collection and calls their evaluate method.
     *          This allows each item to perform its own evaluation and update its state accordingly.
     * @note Derived classes are allowed to override this method to provide custom evaluation logic.
     */
    void evaluate() override {
        CollectionBase::evaluate();    // Evaluate all axes in the collection
        Observable::notifyObservers(); // Notify observers of changes in the axis collection
    };

    /**
     * @brief Attach an observer to all Axes in the collection.
     */
    void attachAxesObserver(IObserver *observer);

    /**
     * @brief Accept the Visitor for all Axes in the collection.
     */
    void accept(IVisitor &visitor) override;
};
