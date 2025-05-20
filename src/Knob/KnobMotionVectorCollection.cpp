#include "KnobMotionVectorCollection.hpp"
#include <Knob/MotionVector/KnobRotation.hpp>
#include <Knob/MotionVector/KnobTranslation.hpp>

/**
 * @brief Setup the axis collection according to the configuration.
 * @details This method initializes the axes based on the configuration defined in config.h.
 *          It creates instances of the axes and sets their context to this KnobMotionVectorCollection instance.
 */
void KnobMotionVectorCollection::setup(ISensorsCalculator *sensorsCalculator) {
    m_items[TRANSX] = new KnobTranslation(TRANSX, sensorsCalculator);
    m_items[TRANSY] = new KnobTranslation(TRANSY, sensorsCalculator);
    m_items[TRANSZ] = new KnobTranslation(TRANSZ, sensorsCalculator);
    m_items[ROTX] = new KnobRotation(ROTX, sensorsCalculator);
    m_items[ROTY] = new KnobRotation(ROTY, sensorsCalculator);
    m_items[ROTZ] = new KnobRotation(ROTZ, sensorsCalculator);
    m_itemCount = 6;
};

#if 0 // REMOVE - Obseervers are attached to kinematicsvectors
void KnobMotionVectorCollection::setup(ISensorsCalculator *sensorsCalculator, IObserver *hidEventBufferTranslation, IObserver *hidEventBufferRotation) {
    // Call the setup function to initialize the axes
    setup(sensorsCalculator);

// Attach the HIDEventBuffers to the axes

    static_cast<KnobTranslation *>(m_items[TRANSX])->attachObserver(hidEventBufferTranslation);
    static_cast<KnobTranslation *>(m_items[TRANSY])->attachObserver(hidEventBufferTranslation);
    static_cast<KnobTranslation *>(m_items[TRANSZ])->attachObserver(hidEventBufferTranslation);
    static_cast<KnobRotation *>(m_items[ROTX])->attachObserver(hidEventBufferRotation);
    static_cast<KnobRotation *>(m_items[ROTY])->attachObserver(hidEventBufferRotation);
    static_cast<KnobRotation *>(m_items[ROTZ])->attachObserver(hidEventBufferRotation);
}
#endif
/**
 * @brief Get the axis at the specified index.
 * @param id Index of the axis to retrieve.
 * @return Pointer to the axis at the specified index, or nullptr if the index is out of bounds.
 * @note The base class Collection::getItem() can be used too, but it returns a pointer to the ICollectable interface.
 */
KnobMotionVector *KnobMotionVectorCollection::getMotionVector(uint8_t id) const {
    return static_cast<KnobMotionVector *>(getItem(id)); // Return the axis with the specified id
}

/**
 * @brief Get the axis with the specified name.
 * @param name Name of the axis to retrieve.
 * @return Pointer to the axis with the specified name, or nullptr if not found.
 * @note The base class method 'Collection::getItem()' can be used too, but it returns a pointer to the ICollectable interface.
 */
KnobMotionVector *KnobMotionVectorCollection::getMotionVector(const char *name) const {
    return static_cast<KnobMotionVector *>(getItem(name));
}

/**
 * @brief Attach an observer to all axes in the collection.
 * @details This function iterates through all axes in the collection and attaches the provided observer to each axis.
 *          This allows the observer to receive updates from all axes in the collection.
 * @param observer Pointer to the observer to be attached.
 * @deprecated This function is not used in the current implementation and may be removed in future versions.
 */
void KnobMotionVectorCollection::attachAxesObserver(IObserver *observer) {
    for (int i = 0; i < m_itemCount; i++) {
        static_cast<KnobMotionVector *>(m_items[i])->attachObserver(observer); // Attach the observer to each axis in the collection
    }
};

/**
 * @brief Distribute the printer visitor to all axes in the collection
 * @param printerVisitor Reference to the printer visitor to be used for printing.
 * @details This function iterates through all axes in the collection and calls the accept method on each axis,
 */
void KnobMotionVectorCollection::acceptAxesVisitor(IPrinterVisitor &printerVisitor) {
    for (int i = 0; i < m_itemCount; i++) {
        static_cast<KnobMotionVector *>(m_items[i])->accept(printerVisitor);
    }
}