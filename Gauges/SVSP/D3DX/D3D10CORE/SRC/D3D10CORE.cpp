#include <Windows.h>
#include <D3D10.h>
#include <DXGI.h>

class OverlayedID3D10Device : public ID3D10Device
{
  ID3D10Device *m_pkDevice;

public:
  OverlayedID3D10Device(ID3D10Device *pkDevice)
  {
    m_pkDevice = pkDevice;
  }

  virtual HRESULT STDMETHODCALLTYPE QueryInterface( REFIID riid, __RPC__deref_out void __RPC_FAR *__RPC_FAR *ppvObject) 
  {
    return m_pkDevice->QueryInterface(riid, ppvObject);
  }

  virtual ULONG STDMETHODCALLTYPE AddRef( void) 
  {
    return m_pkDevice->AddRef();
  }

  virtual ULONG STDMETHODCALLTYPE Release( void)
  {
    return m_pkDevice->Release();
  }

  virtual void STDMETHODCALLTYPE VSSetConstantBuffers(
    /*  */ 
    __in_range( 0, D3D10_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
    /*  */ 
    __in_range( 0, D3D10_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
    /*  */ 
    __in_ecount(NumBuffers)  ID3D10Buffer *const *ppConstantBuffers)
  {
    return m_pkDevice->VSSetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
  }

  virtual void STDMETHODCALLTYPE PSSetShaderResources( 
    /*  */ 
    __in_range( 0, D3D10_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
    /*  */ 
    __in_range( 0, D3D10_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
    /*  */ 
    __in_ecount(NumViews)  ID3D10ShaderResourceView *const *ppShaderResourceViews){}

  virtual void STDMETHODCALLTYPE PSSetShader( 
    /*  */ 
    __in_opt  ID3D10PixelShader *pPixelShader){}

  virtual void STDMETHODCALLTYPE PSSetSamplers( 
    /*  */ 
    __in_range( 0, D3D10_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
    /*  */ 
    __in_range( 0, D3D10_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
    /*  */ 
    __in_ecount(NumSamplers)  ID3D10SamplerState *const *ppSamplers){}

  virtual void STDMETHODCALLTYPE VSSetShader( 
    /*  */ 
    __in_opt  ID3D10VertexShader *pVertexShader){}

  virtual void STDMETHODCALLTYPE DrawIndexed( 
    /*  */ 
    __in  UINT IndexCount,
    /*  */ 
    __in  UINT StartIndexLocation,
    /*  */ 
    __in  INT BaseVertexLocation){}

  virtual void STDMETHODCALLTYPE Draw( 
    /*  */ 
    __in  UINT VertexCount,
    /*  */ 
    __in  UINT StartVertexLocation){}

  virtual void STDMETHODCALLTYPE PSSetConstantBuffers( 
    /*  */ 
    __in_range( 0, D3D10_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
    /*  */ 
    __in_range( 0, D3D10_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
    /*  */ 
    __in_ecount(NumBuffers)  ID3D10Buffer *const *ppConstantBuffers){}

  virtual void STDMETHODCALLTYPE IASetInputLayout( 
    /*  */ 
    __in_opt  ID3D10InputLayout *pInputLayout){}

  virtual void STDMETHODCALLTYPE IASetVertexBuffers( 
    /*  */ 
    __in_range( 0, D3D10_1_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
    /*  */ 
    __in_range( 0, D3D10_1_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumBuffers,
    /*  */ 
    __in_ecount(NumBuffers)  ID3D10Buffer *const *ppVertexBuffers,
    /*  */ 
    __in_ecount(NumBuffers)  const UINT *pStrides,
    /*  */ 
    __in_ecount(NumBuffers)  const UINT *pOffsets){}

  virtual void STDMETHODCALLTYPE IASetIndexBuffer( 
    /*  */ 
    __in_opt  ID3D10Buffer *pIndexBuffer,
    /*  */ 
    __in  DXGI_FORMAT Format,
    /*  */ 
    __in  UINT Offset){}

  virtual void STDMETHODCALLTYPE DrawIndexedInstanced( 
    /*  */ 
    __in  UINT IndexCountPerInstance,
    /*  */ 
    __in  UINT InstanceCount,
    /*  */ 
    __in  UINT StartIndexLocation,
    /*  */ 
    __in  INT BaseVertexLocation,
    /*  */ 
    __in  UINT StartInstanceLocation){}

  virtual void STDMETHODCALLTYPE DrawInstanced( 
    /*  */ 
    __in  UINT VertexCountPerInstance,
    /*  */ 
    __in  UINT InstanceCount,
    /*  */ 
    __in  UINT StartVertexLocation,
    /*  */ 
    __in  UINT StartInstanceLocation){}

  virtual void STDMETHODCALLTYPE GSSetConstantBuffers( 
    /*  */ 
    __in_range( 0, D3D10_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
    /*  */ 
    __in_range( 0, D3D10_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
    /*  */ 
    __in_ecount(NumBuffers)  ID3D10Buffer *const *ppConstantBuffers){}

  virtual void STDMETHODCALLTYPE GSSetShader( 
    /*  */ 
    __in_opt  ID3D10GeometryShader *pShader){}

  virtual void STDMETHODCALLTYPE IASetPrimitiveTopology( 
    /*  */ 
    __in  D3D10_PRIMITIVE_TOPOLOGY Topology){}

  virtual void STDMETHODCALLTYPE VSSetShaderResources( 
    /*  */ 
    __in_range( 0, D3D10_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
    /*  */ 
    __in_range( 0, D3D10_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
    /*  */ 
    __in_ecount(NumViews)  ID3D10ShaderResourceView *const *ppShaderResourceViews){}

  virtual void STDMETHODCALLTYPE VSSetSamplers( 
    /*  */ 
    __in_range( 0, D3D10_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
    /*  */ 
    __in_range( 0, D3D10_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
    /*  */ 
    __in_ecount(NumSamplers)  ID3D10SamplerState *const *ppSamplers){}

  virtual void STDMETHODCALLTYPE SetPredication( 
    /*  */ 
    __in_opt  ID3D10Predicate *pPredicate,
    /*  */ 
    __in  BOOL PredicateValue){}

  virtual void STDMETHODCALLTYPE GSSetShaderResources( 
    /*  */ 
    __in_range( 0, D3D10_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
    /*  */ 
    __in_range( 0, D3D10_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
    /*  */ 
    __in_ecount(NumViews)  ID3D10ShaderResourceView *const *ppShaderResourceViews){}

  virtual void STDMETHODCALLTYPE GSSetSamplers( 
    /*  */ 
    __in_range( 0, D3D10_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
    /*  */ 
    __in_range( 0, D3D10_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
    /*  */ 
    __in_ecount(NumSamplers)  ID3D10SamplerState *const *ppSamplers){}

  virtual void STDMETHODCALLTYPE OMSetRenderTargets( 
    /*  */ 
    __in_range( 0, D3D10_SIMULTANEOUS_RENDER_TARGET_COUNT )  UINT NumViews,
    /*  */ 
    __in_ecount_opt(NumViews)  ID3D10RenderTargetView *const *ppRenderTargetViews,
    /*  */ 
    __in_opt  ID3D10DepthStencilView *pDepthStencilView){}

  virtual void STDMETHODCALLTYPE OMSetBlendState( 
    /*  */ 
    __in_opt  ID3D10BlendState *pBlendState,
    /*  */ 
    __in  const FLOAT BlendFactor[ 4 ],
    /*  */ 
    __in  UINT SampleMask){}

  virtual void STDMETHODCALLTYPE OMSetDepthStencilState( 
    /*  */ 
    __in_opt  ID3D10DepthStencilState *pDepthStencilState,
    /*  */ 
    __in  UINT StencilRef){}

  virtual void STDMETHODCALLTYPE SOSetTargets( 
    /*  */ 
    __in_range( 0, D3D10_SO_BUFFER_SLOT_COUNT)  UINT NumBuffers,
    /*  */ 
    __in_ecount_opt(NumBuffers)  ID3D10Buffer *const *ppSOTargets,
    /*  */ 
    __in_ecount_opt(NumBuffers)  const UINT *pOffsets){}

  virtual void STDMETHODCALLTYPE DrawAuto( void){}

  virtual void STDMETHODCALLTYPE RSSetState( 
    /*  */ 
    __in_opt  ID3D10RasterizerState *pRasterizerState){}

  virtual void STDMETHODCALLTYPE RSSetViewports( 
    /*  */ 
    __in_range(0, D3D10_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE)  UINT NumViewports,
    /*  */ 
    __in_ecount_opt(NumViewports)  const D3D10_VIEWPORT *pViewports){}

  virtual void STDMETHODCALLTYPE RSSetScissorRects( 
    /*  */ 
    __in_range(0, D3D10_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE)  UINT NumRects,
    /*  */ 
    __in_ecount_opt(NumRects)  const D3D10_RECT *pRects){}

  virtual void STDMETHODCALLTYPE CopySubresourceRegion( 
    /*  */ 
    __in  ID3D10Resource *pDstResource,
    /*  */ 
    __in  UINT DstSubresource,
    /*  */ 
    __in  UINT DstX,
    /*  */ 
    __in  UINT DstY,
    /*  */ 
    __in  UINT DstZ,
    /*  */ 
    __in  ID3D10Resource *pSrcResource,
    /*  */ 
    __in  UINT SrcSubresource,
    /*  */ 
    __in_opt  const D3D10_BOX *pSrcBox){}

  virtual void STDMETHODCALLTYPE CopyResource( 
    /*  */ 
    __in  ID3D10Resource *pDstResource,
    /*  */ 
    __in  ID3D10Resource *pSrcResource){}

  virtual void STDMETHODCALLTYPE UpdateSubresource( 
    /*  */ 
    __in  ID3D10Resource *pDstResource,
    /*  */ 
    __in  UINT DstSubresource,
    /*  */ 
    __in_opt  const D3D10_BOX *pDstBox,
    /*  */ 
    __in  const void *pSrcData,
    /*  */ 
    __in  UINT SrcRowPitch,
    /*  */ 
    __in  UINT SrcDepthPitch){}

  virtual void STDMETHODCALLTYPE ClearRenderTargetView( 
    /*  */ 
    __in  ID3D10RenderTargetView *pRenderTargetView,
    /*  */ 
    __in  const FLOAT ColorRGBA[ 4 ]){}

  virtual void STDMETHODCALLTYPE ClearDepthStencilView( 
    /*  */ 
    __in  ID3D10DepthStencilView *pDepthStencilView,
    /*  */ 
    __in  UINT ClearFlags,
    /*  */ 
    __in  FLOAT Depth,
    /*  */ 
    __in  UINT8 Stencil){}

  virtual void STDMETHODCALLTYPE GenerateMips( 
    /*  */ 
    __in  ID3D10ShaderResourceView *pShaderResourceView){}

  virtual void STDMETHODCALLTYPE ResolveSubresource( 
    /*  */ 
    __in  ID3D10Resource *pDstResource,
    /*  */ 
    __in  UINT DstSubresource,
    /*  */ 
    __in  ID3D10Resource *pSrcResource,
    /*  */ 
    __in  UINT SrcSubresource,
    /*  */ 
    __in  DXGI_FORMAT Format){}

  virtual void STDMETHODCALLTYPE VSGetConstantBuffers( 
    /*  */ 
    __in_range( 0, D3D10_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
    /*  */ 
    __in_range( 0, D3D10_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
    /*  */ 
    __out_ecount(NumBuffers)  ID3D10Buffer **ppConstantBuffers){}

  virtual void STDMETHODCALLTYPE PSGetShaderResources( 
    /*  */ 
    __in_range( 0, D3D10_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
    /*  */ 
    __in_range( 0, D3D10_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
    /*  */ 
    __out_ecount(NumViews)  ID3D10ShaderResourceView **ppShaderResourceViews){}

  virtual void STDMETHODCALLTYPE PSGetShader( 
    /*  */ 
    __out  ID3D10PixelShader **ppPixelShader){}

  virtual void STDMETHODCALLTYPE PSGetSamplers( 
    /*  */ 
    __in_range( 0, D3D10_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
    /*  */ 
    __in_range( 0, D3D10_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
    /*  */ 
    __out_ecount(NumSamplers)  ID3D10SamplerState **ppSamplers){}

  virtual void STDMETHODCALLTYPE VSGetShader( 
    /*  */ 
    __out  ID3D10VertexShader **ppVertexShader){}

  virtual void STDMETHODCALLTYPE PSGetConstantBuffers( 
    /*  */ 
    __in_range( 0, D3D10_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
    /*  */ 
    __in_range( 0, D3D10_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
    /*  */ 
    __out_ecount(NumBuffers)  ID3D10Buffer **ppConstantBuffers){}

  virtual void STDMETHODCALLTYPE IAGetInputLayout( 
    /*  */ 
    __out  ID3D10InputLayout **ppInputLayout){}

  virtual void STDMETHODCALLTYPE IAGetVertexBuffers( 
    /*  */ 
    __in_range( 0, D3D10_1_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
    /*  */ 
    __in_range( 0, D3D10_1_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumBuffers,
    /*  */ 
    __out_ecount_opt(NumBuffers)  ID3D10Buffer **ppVertexBuffers,
    /*  */ 
    __out_ecount_opt(NumBuffers)  UINT *pStrides,
    /*  */ 
    __out_ecount_opt(NumBuffers)  UINT *pOffsets){}

  virtual void STDMETHODCALLTYPE IAGetIndexBuffer( 
    /*  */ 
    __out_opt  ID3D10Buffer **pIndexBuffer,
    /*  */ 
    __out_opt  DXGI_FORMAT *Format,
    /*  */ 
    __out_opt  UINT *Offset){}

  virtual void STDMETHODCALLTYPE GSGetConstantBuffers( 
    /*  */ 
    __in_range( 0, D3D10_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
    /*  */ 
    __in_range( 0, D3D10_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
    /*  */ 
    __out_ecount(NumBuffers)  ID3D10Buffer **ppConstantBuffers){}

  virtual void STDMETHODCALLTYPE GSGetShader( 
    /*  */ 
    __out  ID3D10GeometryShader **ppGeometryShader){}

  virtual void STDMETHODCALLTYPE IAGetPrimitiveTopology( 
    /*  */ 
    __out  D3D10_PRIMITIVE_TOPOLOGY *pTopology){}

  virtual void STDMETHODCALLTYPE VSGetShaderResources( 
    /*  */ 
    __in_range( 0, D3D10_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
    /*  */ 
    __in_range( 0, D3D10_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
    /*  */ 
    __out_ecount(NumViews)  ID3D10ShaderResourceView **ppShaderResourceViews){}

  virtual void STDMETHODCALLTYPE VSGetSamplers( 
    /*  */ 
    __in_range( 0, D3D10_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
    /*  */ 
    __in_range( 0, D3D10_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
    /*  */ 
    __out_ecount(NumSamplers)  ID3D10SamplerState **ppSamplers){}

  virtual void STDMETHODCALLTYPE GetPredication( 
    /*  */ 
    __out_opt  ID3D10Predicate **ppPredicate,
    /*  */ 
    __out_opt  BOOL *pPredicateValue){}

  virtual void STDMETHODCALLTYPE GSGetShaderResources( 
    /*  */ 
    __in_range( 0, D3D10_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
    /*  */ 
    __in_range( 0, D3D10_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
    /*  */ 
    __out_ecount(NumViews)  ID3D10ShaderResourceView **ppShaderResourceViews){}

  virtual void STDMETHODCALLTYPE GSGetSamplers( 
    /*  */ 
    __in_range( 0, D3D10_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
    /*  */ 
    __in_range( 0, D3D10_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
    /*  */ 
    __out_ecount(NumSamplers)  ID3D10SamplerState **ppSamplers){}

  virtual void STDMETHODCALLTYPE OMGetRenderTargets( 
    /*  */ 
    __in_range( 0, D3D10_SIMULTANEOUS_RENDER_TARGET_COUNT )  UINT NumViews,
    /*  */ 
    __out_ecount_opt(NumViews)  ID3D10RenderTargetView **ppRenderTargetViews,
    /*  */ 
    __out_opt  ID3D10DepthStencilView **ppDepthStencilView){}

  virtual void STDMETHODCALLTYPE OMGetBlendState( 
    /*  */ 
    __out_opt  ID3D10BlendState **ppBlendState,
    /*  */ 
    __out_opt  FLOAT BlendFactor[ 4 ],
    /*  */ 
    __out_opt  UINT *pSampleMask){}

  virtual void STDMETHODCALLTYPE OMGetDepthStencilState( 
    /*  */ 
    __out_opt  ID3D10DepthStencilState **ppDepthStencilState,
    /*  */ 
    __out_opt  UINT *pStencilRef){}

  virtual void STDMETHODCALLTYPE SOGetTargets( 
    /*  */ 
    __in_range( 0, D3D10_SO_BUFFER_SLOT_COUNT )  UINT NumBuffers,
    /*  */ 
    __out_ecount_opt(NumBuffers)  ID3D10Buffer **ppSOTargets,
    /*  */ 
    __out_ecount_opt(NumBuffers)  UINT *pOffsets){}

  virtual void STDMETHODCALLTYPE RSGetState( 
    /*  */ 
    __out  ID3D10RasterizerState **ppRasterizerState){}

  virtual void STDMETHODCALLTYPE RSGetViewports( 
    /*  */ 
    __inout /*_range(0, D3D10_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE )*/   UINT *NumViewports,
    /*  */ 
    __out_ecount_opt(NumViewports)  D3D10_VIEWPORT *pViewports){}

  virtual void STDMETHODCALLTYPE RSGetScissorRects( 
    /*  */ 
    __inout /*_range(0, D3D10_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE )*/   UINT *NumRects,
    /*  */ 
    __out_ecount_opt(NumRects)  D3D10_RECT *pRects){}

  virtual HRESULT STDMETHODCALLTYPE GetDeviceRemovedReason( void)
  {
    return m_pkDevice->GetDeviceRemovedReason();
  }

  virtual HRESULT STDMETHODCALLTYPE SetExceptionMode( 
    UINT RaiseFlags)
  {
    return m_pkDevice->SetExceptionMode(RaiseFlags);
  }

  virtual UINT STDMETHODCALLTYPE GetExceptionMode( void)
  {
    return m_pkDevice->GetExceptionMode();
  }

  virtual HRESULT STDMETHODCALLTYPE GetPrivateData( 
    /*  */ 
    __in  REFGUID guid,
    /*  */ 
    __inout  UINT *pDataSize,
    /*  */ 
    __out_bcount_opt(*pDataSize)  void *pData)
  {
    return m_pkDevice->GetPrivateData(guid, pDataSize, pData);
  }

  virtual HRESULT STDMETHODCALLTYPE SetPrivateData( 
    /*  */ 
    __in  REFGUID guid,
    /*  */ 
    __in  UINT DataSize,
    /*  */ 
    __in_bcount_opt(DataSize)  const void *pData)
  {
    return m_pkDevice->SetPrivateData(guid, DataSize, pData);
  }

  virtual HRESULT STDMETHODCALLTYPE SetPrivateDataInterface( 
    /*  */ 
    __in  REFGUID guid,
    /*  */ 
    __in_opt  const IUnknown *pData)
  {
    return m_pkDevice->SetPrivateDataInterface(guid, pData);
  }

  virtual void STDMETHODCALLTYPE ClearState( void){}

  virtual void STDMETHODCALLTYPE Flush( void){}

  virtual HRESULT STDMETHODCALLTYPE CreateBuffer( 
    /*  */ 
    __in  const D3D10_BUFFER_DESC *pDesc,
    /*  */ 
    __in_opt  const D3D10_SUBRESOURCE_DATA *pInitialData,
    /*  */ 
    __out_opt  ID3D10Buffer **ppBuffer)
  {
    return m_pkDevice->CreateBuffer(pDesc, pInitialData, ppBuffer);
  }

  virtual HRESULT STDMETHODCALLTYPE CreateTexture1D( 
    /*  */ 
    __in  const D3D10_TEXTURE1D_DESC *pDesc,
    /*  */ 
    __in_xcount_opt(pDesc->MipLevels * pDesc->ArraySize)  const D3D10_SUBRESOURCE_DATA *pInitialData,
    /*  */ 
    __out  ID3D10Texture1D **ppTexture1D)
  {
    return m_pkDevice->CreateTexture1D(pDesc, pInitialData, ppTexture1D);
  }

  virtual HRESULT STDMETHODCALLTYPE CreateTexture2D( 
    /*  */ 
    __in  const D3D10_TEXTURE2D_DESC *pDesc,
    /*  */ 
    __in_xcount_opt(pDesc->MipLevels * pDesc->ArraySize)  const D3D10_SUBRESOURCE_DATA *pInitialData,
    /*  */ 
    __out  ID3D10Texture2D **ppTexture2D)
  {
    return m_pkDevice->CreateTexture2D(pDesc, pInitialData, ppTexture2D);
  }

  virtual HRESULT STDMETHODCALLTYPE CreateTexture3D( 
    /*  */ 
    __in  const D3D10_TEXTURE3D_DESC *pDesc,
    /*  */ 
    __in_xcount_opt(pDesc->MipLevels)  const D3D10_SUBRESOURCE_DATA *pInitialData,
    /*  */ 
    __out  ID3D10Texture3D **ppTexture3D)
  {
    return m_pkDevice->CreateTexture3D(pDesc, pInitialData, ppTexture3D);
  }

  virtual HRESULT STDMETHODCALLTYPE CreateShaderResourceView( 
    /*  */ 
    __in  ID3D10Resource *pResource,
    /*  */ 
    __in_opt  const D3D10_SHADER_RESOURCE_VIEW_DESC *pDesc,
    /*  */ 
    __out_opt  ID3D10ShaderResourceView **ppSRView)
  {
    return m_pkDevice->CreateShaderResourceView(pResource, pDesc, ppSRView);
  }

  virtual HRESULT STDMETHODCALLTYPE CreateRenderTargetView( 
    /*  */ 
    __in  ID3D10Resource *pResource,
    /*  */ 
    __in_opt  const D3D10_RENDER_TARGET_VIEW_DESC *pDesc,
    /*  */ 
    __out_opt  ID3D10RenderTargetView **ppRTView)
  {
    return m_pkDevice->CreateRenderTargetView(pResource, pDesc, ppRTView);
  }

  virtual HRESULT STDMETHODCALLTYPE CreateDepthStencilView( 
    /*  */ 
    __in  ID3D10Resource *pResource,
    /*  */ 
    __in_opt  const D3D10_DEPTH_STENCIL_VIEW_DESC *pDesc,
    /*  */ 
    __out_opt  ID3D10DepthStencilView **ppDepthStencilView)
  {
    return m_pkDevice->CreateDepthStencilView(pResource, pDesc, ppDepthStencilView);
  }

  virtual HRESULT STDMETHODCALLTYPE CreateInputLayout( 
    /*  */ 
    __in_ecount(NumElements)  const D3D10_INPUT_ELEMENT_DESC *pInputElementDescs,
    /*  */ 
    __in_range( 0, D3D10_1_IA_VERTEX_INPUT_STRUCTURE_ELEMENT_COUNT )  UINT NumElements,
    /*  */ 
    __in  const void *pShaderBytecodeWithInputSignature,
    /*  */ 
    __in  SIZE_T BytecodeLength,
    /*  */ 
    __out_opt  ID3D10InputLayout **ppInputLayout)
  {
    return m_pkDevice->CreateInputLayout(pInputElementDescs, NumElements, pShaderBytecodeWithInputSignature, BytecodeLength, ppInputLayout);
  }

  virtual HRESULT STDMETHODCALLTYPE CreateVertexShader( 
    /*  */ 
    __in  const void *pShaderBytecode,
    /*  */ 
    __in  SIZE_T BytecodeLength,
    /*  */ 
    __out_opt  ID3D10VertexShader **ppVertexShader)
  {
    return m_pkDevice->CreateVertexShader(pShaderBytecode, BytecodeLength, ppVertexShader);
  }

  virtual HRESULT STDMETHODCALLTYPE CreateGeometryShader( 
    /*  */ 
    __in  const void *pShaderBytecode,
    /*  */ 
    __in  SIZE_T BytecodeLength,
    /*  */ 
    __out_opt  ID3D10GeometryShader **ppGeometryShader)
  {
    return m_pkDevice->CreateGeometryShader(pShaderBytecode, BytecodeLength, ppGeometryShader);
  }

  virtual HRESULT STDMETHODCALLTYPE CreateGeometryShaderWithStreamOutput( 
    /*  */ 
    __in  const void *pShaderBytecode,
    /*  */ 
    __in  SIZE_T BytecodeLength,
    /*  */ 
    __in_ecount_opt(NumEntries)  const D3D10_SO_DECLARATION_ENTRY *pSODeclaration,
    /*  */ 
    __in_range( 0, D3D10_SO_SINGLE_BUFFER_COMPONENT_LIMIT )  UINT NumEntries,
    /*  */ 
    __in  UINT OutputStreamStride,
    /*  */ 
    __out_opt  ID3D10GeometryShader **ppGeometryShader)
  {
    return m_pkDevice->CreateGeometryShaderWithStreamOutput(pShaderBytecode, BytecodeLength, pSODeclaration, NumEntries, OutputStreamStride, ppGeometryShader);
  }

  virtual HRESULT STDMETHODCALLTYPE CreatePixelShader( 
    /*  */ 
    __in  const void *pShaderBytecode,
    /*  */ 
    __in  SIZE_T BytecodeLength,
    /*  */ 
    __out_opt  ID3D10PixelShader **ppPixelShader)
  {
    return m_pkDevice->CreatePixelShader(pShaderBytecode, BytecodeLength, ppPixelShader);
  }

  virtual HRESULT STDMETHODCALLTYPE CreateBlendState( 
    /*  */ 
    __in  const D3D10_BLEND_DESC *pBlendStateDesc,
    /*  */ 
    __out_opt  ID3D10BlendState **ppBlendState)
  {
    return m_pkDevice->CreateBlendState(pBlendStateDesc, ppBlendState);
  }

  virtual HRESULT STDMETHODCALLTYPE CreateDepthStencilState( 
    /*  */ 
    __in  const D3D10_DEPTH_STENCIL_DESC *pDepthStencilDesc,
    /*  */ 
    __out_opt  ID3D10DepthStencilState **ppDepthStencilState)
  {
    return m_pkDevice->CreateDepthStencilState(pDepthStencilDesc, ppDepthStencilState);
  }

  virtual HRESULT STDMETHODCALLTYPE CreateRasterizerState( 
    /*  */ 
    __in  const D3D10_RASTERIZER_DESC *pRasterizerDesc,
    /*  */ 
    __out_opt  ID3D10RasterizerState **ppRasterizerState)
  {
    return m_pkDevice->CreateRasterizerState(pRasterizerDesc, ppRasterizerState);
  }

  virtual HRESULT STDMETHODCALLTYPE CreateSamplerState( 
    /*  */ 
    __in  const D3D10_SAMPLER_DESC *pSamplerDesc,
    /*  */ 
    __out_opt  ID3D10SamplerState **ppSamplerState)
  {
    return m_pkDevice->CreateSamplerState(pSamplerDesc, ppSamplerState);
  }

  virtual HRESULT STDMETHODCALLTYPE CreateQuery( 
    /*  */ 
    __in  const D3D10_QUERY_DESC *pQueryDesc,
    /*  */ 
    __out_opt  ID3D10Query **ppQuery)
  {
    return m_pkDevice->CreateQuery(pQueryDesc, ppQuery);
  }

  virtual HRESULT STDMETHODCALLTYPE CreatePredicate( 
    /*  */ 
    __in  const D3D10_QUERY_DESC *pPredicateDesc,
    /*  */ 
    __out_opt  ID3D10Predicate **ppPredicate)
  {
    return m_pkDevice->CreatePredicate(pPredicateDesc, ppPredicate);
  }

  virtual HRESULT STDMETHODCALLTYPE CreateCounter( 
    /*  */ 
    __in  const D3D10_COUNTER_DESC *pCounterDesc,
    /*  */ 
    __out_opt  ID3D10Counter **ppCounter)
  {
    return m_pkDevice->CreateCounter(pCounterDesc, ppCounter);
  }

  virtual HRESULT STDMETHODCALLTYPE CheckFormatSupport( 
    /*  */ 
    __in  DXGI_FORMAT Format,
    /*  */ 
    __out  UINT *pFormatSupport)
  {
    return m_pkDevice->CheckFormatSupport(Format, pFormatSupport);
  }

  virtual HRESULT STDMETHODCALLTYPE CheckMultisampleQualityLevels( 
    /*  */ 
    __in  DXGI_FORMAT Format,
    /*  */ 
    __in  UINT SampleCount,
    /*  */ 
    __out  UINT *pNumQualityLevels)
  {
    return m_pkDevice->CheckMultisampleQualityLevels(Format, SampleCount, pNumQualityLevels);
  }

  virtual void STDMETHODCALLTYPE CheckCounterInfo( 
    /*  */ 
    __out  D3D10_COUNTER_INFO *pCounterInfo){}

  virtual HRESULT STDMETHODCALLTYPE CheckCounter( 
    /*  */ 
    __in  const D3D10_COUNTER_DESC *pDesc,
    /*  */ 
    __out  D3D10_COUNTER_TYPE *pType,
    /*  */ 
    __out  UINT *pActiveCounters,
    /*  */ 
    __out_ecount_opt(*pNameLength)  LPSTR szName,
    /*  */ 
    __inout_opt  UINT *pNameLength,
    /*  */ 
    __out_ecount_opt(*pUnitsLength)  LPSTR szUnits,
    /*  */ 
    __inout_opt  UINT *pUnitsLength,
    /*  */ 
    __out_ecount_opt(*pDescriptionLength)  LPSTR szDescription,
    /*  */ 
    __inout_opt  UINT *pDescriptionLength)
  {
    return m_pkDevice->CheckCounter(pDesc, pType, pActiveCounters, szName, pNameLength, szUnits, pUnitsLength, szDescription, pDescriptionLength);
  }

  virtual UINT STDMETHODCALLTYPE GetCreationFlags( void)
  {
    return m_pkDevice->GetCreationFlags();
  }

  virtual HRESULT STDMETHODCALLTYPE OpenSharedResource( 
    /*  */ 
    __in  HANDLE hResource,
    /*  */ 
    __in  REFIID ReturnedInterface,
    /*  */ 
    __out_opt  void **ppResource)
  {
    return m_pkDevice->OpenSharedResource(hResource, ReturnedInterface, ppResource);
  }

  virtual void STDMETHODCALLTYPE SetTextFilterSize( 
    /*  */ 
    __in  UINT Width,
    /*  */ 
    __in  UINT Height){}

  virtual void STDMETHODCALLTYPE GetTextFilterSize( 
    /*  */ 
    __out_opt  UINT *pWidth,
    /*  */ 
    __out_opt  UINT *pHeight){}

};




#define DllEXPORT __declspec( dllexport )
extern "C" HRESULT WINAPI OpenAdapter10(VOID *pOpenData); 
extern "C" HRESULT WINAPI D3D10CoreCreateDevice(IDXGIFactory *pFactory, IDXGIAdapter *pAdapter, UINT uiFlags, DWORD dwUnknown0, ID3D10Device **ppDevice); 
extern "C" HRESULT WINAPI D3D10CoreGetVersion(UINT *pVersion);
extern "C" HRESULT WINAPI D3D10CoreRegisterLayers(const struct dxgi_device_layer *layers, UINT layer_count);

extern "C" HRESULT WINAPI OpenAdapter10(VOID *pOpenData)
{
  HRESULT hResult = S_FALSE;

  return hResult;
}

extern "C" HRESULT WINAPI D3D10CoreCreateDevice(IDXGIFactory *pFactory, IDXGIAdapter *pAdapter, UINT uiFlags, DWORD dwUnknown0, ID3D10Device **ppDevice)
{
  HRESULT hResult = S_FALSE;
  char    acD3D10COREPath[MAX_PATH];
  HMODULE __D3D10CORE               = NULL;
  FARPROC __D3D10CoreCreateDevice = NULL;

  GetWindowsDirectory(&acD3D10COREPath[0], sizeof(acD3D10COREPath));
  strcat_s(&acD3D10COREPath[0], sizeof(acD3D10COREPath), "\\System32\\d3d10core.dll"); 
  if( (__D3D10CORE = LoadLibrary(&acD3D10COREPath[0])) != NULL)
  {
    if( (__D3D10CoreCreateDevice = GetProcAddress(__D3D10CORE, "D3D10CoreCreateDevice")) != NULL)
    {
      hResult = ((HRESULT (*)(IDXGIFactory *, IDXGIAdapter *, UINT, DWORD, ID3D10Device **))__D3D10CoreCreateDevice)(pFactory, pAdapter, uiFlags, dwUnknown0, ppDevice);
    }
  }

  //  //char    acDXGIPath[MAX_PATH];
  //  //HMODULE __DXGI               = NULL;
  //  //FARPROC __DXGID3D10CreateDevice = NULL;

  //  //GetWindowsDirectory(&acDXGIPath[0], sizeof(acDXGIPath));
  //  //strcat_s(&acDXGIPath[0], sizeof(acDXGIPath), "\\System32\\DXGI.dll"); 
  //  //if( (__DXGI = LoadLibrary(&acDXGIPath[0])) != NULL)
  //  //{
  //  //  if( (__DXGID3D10CreateDevice = GetProcAddress(__DXGI, "DXGID3D10CreateDevice")) != NULL)
  //  //  {
  //  //    hResult = ((HRESULT (*)(HMODULE , IDXGIFactory *, IDXGIAdapter *, UINT , DWORD , void **))__DXGID3D10CreateDevice)(__D3D10CORE, pFactory, pAdapter, uiFlags, dwUnknown0, (VOID **)ppDevice);
  //  //  }
  //  //  //FreeLibrary(__DXGI);
  //  //}

  //  ////FreeLibrary(__D3D10CORE);
  //}

  //ID3D10Device *pkSysDevice = NULL;
  //char    acD3D10Path[MAX_PATH];
  //HMODULE __D3D10              = NULL;
  //FARPROC __D3D10CreateDevice = NULL;

  //GetWindowsDirectory(&acD3D10Path[0], sizeof(acD3D10Path));
  //strcat_s(&acD3D10Path[0], sizeof(acD3D10Path), "\\System32\\D3D10.dll"); 
  //if( (__D3D10 = LoadLibrary(&acD3D10Path[0])) != NULL)
  //{
  //  if( (__D3D10CreateDevice = GetProcAddress(__D3D10, "D3D10CreateDevice")) != NULL)
  //  {
  //    //m_hResult = D3D10CreateDevice( NULL, D3D10_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, D3D10_SDK_VERSION, &m_pkDevice );
  //    hResult = ((HRESULT (*)(IDXGIAdapter *, D3D10_DRIVER_TYPE, HMODULE, UINT, UINT, ID3D10Device **))__D3D10CreateDevice)(NULL, D3D10_DRIVER_TYPE_HARDWARE, NULL, 0, D3D10_SDK_VERSION, &pkSysDevice);
  //  }
  //}

  //if(SUCCEEDED(hResult) && (pkSysDevice != NULL))
  //{
    //*ppDevice = new OverlayedID3D10Device(pkSysDevice);
  //}

  return hResult;
}

extern "C" HRESULT WINAPI D3D10CoreRegisterLayers(const struct dxgi_device_layer *layers, UINT layer_count)
{
  HRESULT hResult = S_FALSE;
  char    acWinDir[MAX_PATH];
  HMODULE __D3D10CORE                  = NULL;
  FARPROC __D3D10CoreRegisterLayers = NULL;

  GetWindowsDirectory(&acWinDir[0], sizeof(acWinDir));
  strcat_s(&acWinDir[0], sizeof(acWinDir), "\\System32\\D3D10CORE.dll"); 
  if( (__D3D10CORE = LoadLibrary(&acWinDir[0])) != NULL)
  {
    if( (__D3D10CoreRegisterLayers = GetProcAddress(__D3D10CORE, "D3D10CoreRegisterLayers")) != NULL)
    {
      hResult = ((HRESULT (*)(const struct dxgi_device_layer *, UINT ))__D3D10CoreRegisterLayers)(layers, layer_count);
    }
  }
  return hResult;
}

extern "C" HRESULT WINAPI D3D10CoreGetVersion(UINT *pVersion)
{
  HRESULT hResult = S_FALSE;
  char    acWinDir[MAX_PATH];
  HMODULE __D3D10CORE                  = NULL;
  FARPROC __D3D10CoreGetVersion = NULL;

  GetWindowsDirectory(&acWinDir[0], sizeof(acWinDir));
  strcat_s(&acWinDir[0], sizeof(acWinDir), "\\System32\\D3D10CORE.dll"); 
  if( (__D3D10CORE = LoadLibrary(&acWinDir[0])) != NULL)
  {
    if( (__D3D10CoreGetVersion = GetProcAddress(__D3D10CORE, "D3D10CoreGetVersion")) != NULL)
    {
      hResult = ((HRESULT (*)(UINT *))__D3D10CoreGetVersion)(pVersion);
    }
  }
  return hResult;
}

BOOL APIENTRY DllMain(HINSTANCE hInst, DWORD fdwReason, LPVOID lpvReserved)
{
  HRESULT hResult;

  switch(fdwReason)
  {
    case DLL_PROCESS_ATTACH:
      break;

    case DLL_PROCESS_DETACH:
      break;

    default:
      break;
  }
  return TRUE;
}
