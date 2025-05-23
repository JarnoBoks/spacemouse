#include "KnobAxisCollection.hpp"

#include <knob/axis/KnobAxis.hpp>
#if 0
//NOTE See AxisBase.hpp for the reason why we don't use this
#include <knob/axis/deprecated/KnobAxisRotation.hpp>
#include <knob/axis/deprecated/KnobAxisTranslation.hpp>
#endif

/**
 * @brief Setup the axis collection according to the configuration.
 * @details This method initializes the axes based on the configuration defined in config.h.
 *          It creates instances of the axes and sets their context to this KnobAxisCollection instance.
 */
void KnobAxisCollection::setup(ISensorsCalculator *sensorsCalculator) {
    m_items[TRANSX] = new KnobAxis(TRANSX, sensorsCalculator);
    m_items[TRANSY] = new KnobAxis(TRANSY, sensorsCalculator);
    m_items[TRANSZ] = new KnobAxis(TRANSZ, sensorsCalculator);
    m_items[ROTX] = new KnobAxis(ROTX, sensorsCalculator);
    m_items[ROTY] = new KnobAxis(ROTY, sensorsCalculator);
    m_items[ROTZ] = new KnobAxis(ROTZ, sensorsCalculator);
    m_itemCount = 6;
};

void KnobAxisCollection::setup(ISensorsCalculator *sensorsCalculator, IObserver *hidEventBufferTranslation, IObserver *hidEventBufferRotation) {
    // Call the setup function to initialize the axes
    setup(sensorsCalculator);

    // Attach the HIDEventBuffers to the axes
    static_cast<KnobAxis *>(m_items[TRANSX])->attachObserver(hidEventBufferTranslation);
    static_cast<KnobAxis *>(m_items[TRANSY])->attachObserver(hidEventBufferTranslation);
    static_cast<KnobAxis *>(m_items[TRANSZ])->attachObserver(hidEventBufferTranslation);
    static_cast<KnobAxis *>(m_items[ROTX])->attachObserver(hidEventBufferRotation);
    static_cast<KnobAxis *>(m_items[ROTY])->attachObserver(hidEventBufferRotation);
    static_cast<KnobAxis *>(m_items[ROTZ])->attachObserver(hidEventBufferRotation);
};

/**
 * @brief Get the axis at the specified index in the collection.
 * @param id Index of the axis to retrieve.
 * @return Pointer to the axis at the specified index, or nullptr if the index is out of bounds.
 * @note The base class CollectionBase::getItem() can be used too, but it returns a pointer to the ICollectable interface.
 */
KnobAxis *KnobAxisCollection::getAxis(uint8_t id) const {
    return static_cast<KnobAxis *>(getItem(id)); // Return the axis with the specified id
}

/**
 * @brief Get the axis with the specified name.
 * @param name Name of the axis to retrieve.
 * @return Pointer to the axis with the specified name, or nullptr if not found.
 * @note The base class method 'CollectionBase::getItem()' can be used too, but it returns a pointer to the ICollectable interface.
 */
KnobAxis *KnobAxisCollection::getAxis(const char *name) const {
    return static_cast<KnobAxis *>(getItem(name));
}

/**
 * @brief Get the axis with the specified type.
 * @param type Type of the axis to retrieve.
 * @return Pointer to the axis with the specified type, or nullptr if not found.
 * @note The base class method 'CollectionBase::getItem()' can be used too, but it returns a pointer to the ICollectable interface.
 */
KnobAxis *KnobAxisCollection::getAxis(const MotionVector_t type) const {
    return static_cast<KnobAxis *>(getItem(type)); // Return the axis with the specified type
}

/**
 * @brief Attach an observer to all axes in the collection.
 * @details This function iterates through all axes in the collection and attaches the provided observer to each axis.
 *          This allows the observer to receive updates from all axes in the collection.
 * @param observer Pointer to the observer to be attached.
 * @deprecated This function is not used in the current implementation and may be removed in future versions.
 */
void KnobAxisCollection::attachAxesObserver(IObserver *observer) {
    for (int i = 0; i < m_itemCount; i++) {
        static_cast<KnobAxis *>(m_items[i])->attachObserver(observer); // Attach the observer to each axis in the collection
    }
};

/**
 * @brief Distribute the printer visitor to all axes in the collection
 * @param printerVisitor Reference to the printer visitor to be used for printing.
 * @details This function iterates through all axes in the collection and calls the accept method on each axis,
 */
void KnobAxisCollection::acceptAxesPrinter(IPrinterVisitor &printerVisitor) {
    for (int i = 0; i < m_itemCount; i++) {
        static_cast<KnobAxis *>(m_items[i])->accept(printerVisitor);
    }
}

/**
 * @brief Distribute the visitor to all axes in the collection
 * @param visitor Reference to the visitor to be used for processing.
 * @details This function iterates through all axes in the collection and calls the accept method on each axis,
 */
void KnobAxisCollection::acceptAxesVisitor(IVisitor &visitor) {
    for (int i = 0; i < m_itemCount; i++) {
        static_cast<KnobAxis *>(m_items[i])->accept(visitor);
    }
}