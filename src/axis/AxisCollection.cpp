#include "AxisCollection.hpp"
#include "axes/AxisRotation.hpp"
#include "axes/AxisTranslation.hpp"

/**
 * @brief Setup the axis collection according to the configuration.
 * @details This method initializes the axes based on the configuration defined in config.h.
 *          It creates instances of the axes and sets their context to this AxisCollection instance.
 */
void AxisCollection::setup(ISensorsCalculator *sensorsCalculator) {
    // REFACTOR - Create an 'add' function that allows for direct adressing the index.
    m_items[TRANSX] = new AxisTranslation(TRANSX, sensorsCalculator);
    m_items[TRANSY] = new AxisTranslation(TRANSY, sensorsCalculator);
    m_items[TRANSZ] = new AxisTranslation(TRANSZ, sensorsCalculator);
    m_items[ROTX] = new AxisRotation(ROTX, sensorsCalculator);
    m_items[ROTY] = new AxisRotation(ROTY, sensorsCalculator);
    m_items[ROTZ] = new AxisRotation(ROTZ, sensorsCalculator);
    m_itemCount = 6;
};

void AxisCollection::setup(ISensorsCalculator *sensorsCalculator, IObserver *hidEventBufferTranslation, IObserver *hidEventBufferRotation) {
    // Call the setup function to initialize the axes
    setup(sensorsCalculator);

    // Attach the HIDEventBuffer to the axes
    static_cast<AxisTranslation *>(m_items[TRANSX])->attachObserver(hidEventBufferTranslation);
    static_cast<AxisTranslation *>(m_items[TRANSY])->attachObserver(hidEventBufferTranslation);
    static_cast<AxisTranslation *>(m_items[TRANSZ])->attachObserver(hidEventBufferTranslation);
    static_cast<AxisRotation *>(m_items[ROTX])->attachObserver(hidEventBufferRotation);
    static_cast<AxisRotation *>(m_items[ROTY])->attachObserver(hidEventBufferRotation);
    static_cast<AxisRotation *>(m_items[ROTZ])->attachObserver(hidEventBufferRotation);
};

/**
 * @brief Get the axis at the specified index.
 * @param id Index of the axis to retrieve.
 * @return Pointer to the axis at the specified index, or nullptr if the index is out of bounds.
 * @note The base class Collection::getItem() can be used too, but it returns a pointer to the ICollectable interface.
 */
Axis *AxisCollection::getAxis(uint8_t id) const {
    return static_cast<Axis *>(getItem(id)); // Return the axis with the specified id
}

/**
 * @brief Get the axis with the specified name.
 * @param name Name of the axis to retrieve.
 * @return Pointer to the axis with the specified name, or nullptr if not found.
 * @note The base class method 'Collection::getItem()' can be used too, but it returns a pointer to the ICollectable interface.
 */
Axis *AxisCollection::getAxis(const char *name) const {
    return static_cast<Axis *>(getItem(name));
}

/**
 * @brief Attach an observer to all axes in the collection.
 * @details This function iterates through all axes in the collection and attaches the provided observer to each axis.
 *          This allows the observer to receive updates from all axes in the collection.
 * @param observer Pointer to the observer to be attached.
 * @deprecated This function is not used in the current implementation and may be removed in future versions.
 */
void AxisCollection::attachAxesObserver(IObserver *observer) {
    for (int i = 0; i < m_itemCount; i++) {
        static_cast<Axis *>(m_items[i])->attachObserver(observer); // Attach the observer to each axis in the collection
    }
};

/**
 * @brief Distribute the printer visitor to all axes in the collection
 * @param printerVisitor Reference to the printer visitor to be used for printing.
 * @details This function iterates through all axes in the collection and calls the accept method on each axis,
 */
void AxisCollection::acceptAxesVisitor(IPrinterVisitor &printerVisitor) {
    for (int i = 0; i < m_itemCount; i++) {
        static_cast<Axis *>(m_items[i])->accept(printerVisitor);
    }
}