# afmp-autotune

Progetto di creazione di plugin autotune per il corso di Algorithms for Music Processing del Politecnico di Torino

## window_maker branch

This branch is dedicated to the creation of a circular buffer that manages the creation of a window.
The window is created by appending consecutive buffers of samples coming from JUCE.
Once the window is full it is sent to the other parts of the plugin that compute the signal analisys operations.
The term "full" is used loosely in the sentence before, the way it really works it's a little bit more complicated because the buffers coming from JUCE are variable in lenght.

The way it works is with a division in two areas of the window: one readble-writable and one to-compute.
A method "will_be_full" returns true if the incoming buffer will exceed the readble-writable area, this permits the plugin to call the get_window_to_elaborate method that send the to-compute area.
The following method set_window_once_elaborate receive the elaborated array of samples and set the to-compute area to readble-writeable.

The buffer_read_and_write method is dedicated to the management of the input-output buffer of the plugin.
Lasly the class include a overload of the operator [] that simplify the use of indexes numbers.
