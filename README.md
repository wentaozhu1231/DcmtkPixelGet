# DcmtkPixelGet
If I want to get no rendered data direct from an image, I can't use getOutputData() from dcm_image since that API only provides 'render data.' The only right way is to use getElement from DcmFormat and use 'element' to convert to Uint16 or something else.
