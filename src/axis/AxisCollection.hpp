#pragma once

#include "common/Collection.hpp"  // Include the ICollection interface header file
#include "common/Observable.hpp"  // Include the ICollection interface header file
#include "visitors/Visitable.hpp" // Include the IVisitable interface header file

#include <stdint.h>

class ISensorsCalculator;
class Axis;
class HIDEventBuffer;
class IPrinterVisitor;

/// @brief Number of axes that can be added to the collection.
/// @details This is a constant value that defines the maximum number of axes that can be added to the collection.
/// @note As long as the specific hardware options for the SpaceMouse all have the same number of axes, this is a good solution.
constexpr uint8_t cHW_MAX_AXES = 6;

/// @brief Number of observers that can be added to this collection.
/// @details This is a constant value that defines the maximum number of observers that can be added to the collection.
constexpr uint8_t c_MAX_AXISCOLLECTION_OBSERVERS = 4;

/**
 * @brief Class representing a visitable,observable collection of axes for the SpaceMouse.
 * @details The class implements the ICollection interface and provides functionality for managing a collection of axes.
 *          The class implements the Observable interface, allowing it to notify observers of changes in the collection.
 *         The class implements the Visitable interface, allowing it to accept visitors.
 * @note The AxisCollection class is designed to manage a fixed number of axes and their associated observers.
 */
class AxisCollection : public Collection, public Observable, public Visitable {
private:
public:
    /// @brief Constructor for empty AxisCollection
    AxisCollection() : Collection(cHW_MAX_AXES), Observable(c_MAX_AXISCOLLECTION_OBSERVERS) {}
    ~AxisCollection() {
    }

    /**
     * @brief Set up the axis collection based on the configuration.
     * @details This function initializes the axis collection and sets up the axes based on the configuration.
     *          It creates instances of the axes and configures them according to the provided configuration.
     */
    void setup(ISensorsCalculator *sensorsCalculator);
    void setup(ISensorsCalculator *sensorsCalculator, IObserver *hidEventBufferTranslation, IObserver *hidEventBufferRotation);

    Axis *getAxis(const uint8_t id) const;
    Axis *getAxis(const char *name) const;

    /**
     * @brief Evaluate all items in the collection and notify observers of the changes.
     * @details This function iterates through all items in the collection and calls their evaluate method.
     *          This allows each item to perform its own evaluation and update its state accordingly.
     * @note Derived classes are allowed to override this method to provide custom evaluation logic.
     */
    void evaluate() override {
        Collection::evaluate();        // Evaluate the axes in the collection
        Observable::notifyObservers(); // Notify observers of changes in the axis collection
    };

    // REVIEW - We should be able to apply a function to all axes in the collection, for now it is programmed manually for the attachment of observers
    /**
     * @brief Attach an observer to all axes in the collection.
     */
    void attachAxesObserver(IObserver *observer);

    /**
     * @brief Accept the Printer Visitor for all axes in the collection.
     */
    void acceptAxesVisitor(IPrinterVisitor &printerVisitor);
};
