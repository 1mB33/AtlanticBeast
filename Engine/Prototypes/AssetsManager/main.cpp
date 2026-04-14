#include <cassert>
#include <chrono>
#include <iostream>
#include <memory>
#include <string_view>
#include <unordered_map>

#define B33_ASSET( className )                                                                                         \
  public:                                                                                                              \
    static ::std::string_view GetAssetTypeName()                                                                       \
    {                                                                                                                  \
        return #className;                                                                                             \
    }                                                                                                                  \
                                                                                                                       \
  private:

// --------------------------------------------------------------------------------------------------------------------
class Mesh
{
    B33_ASSET( Mesh );

  public:
    Mesh()  = default;
    ~Mesh() = default;

    void Action() const
    {
        ::std::cout << "I'm a mesh" << ::std::endl;
    }
};

// --------------------------------------------------------------------------------------------------------------------
class Texture
{
    B33_ASSET( Texture );

  public:
    Texture()  = default;
    ~Texture() = default;
};

// --------------------------------------------------------------------------------------------------------------------
class Sound
{
    B33_ASSET( Sound );

  public:
    Sound()  = default;
    ~Sound() = default;
};

// --------------------------------------------------------------------------------------------------------------------
enum EAssetType
{
    EUnkonwn,
    EMesh,
    ETexture,
    ESound,
};

// --------------------------------------------------------------------------------------------------------------------
struct AssetMetadata
{
    using Clock = ::std::chrono::system_clock;

    EAssetType        Type;
    Clock::time_point CreationTime;
    Clock::time_point LastModificationTime;
};

// --------------------------------------------------------------------------------------------------------------------
class AssetWrapper
{
};

// --------------------------------------------------------------------------------------------------------------------
template <class ASSET_TYPE>
class Asset : public AssetWrapper
{
    using Clock = ::std::chrono::system_clock;

  public:
    Asset( AssetMetadata metadata = { .Type                 = EAssetType::EUnkonwn,
                                      .CreationTime         = Clock::now(),
                                      .LastModificationTime = Clock::now() },
           ::std::string strName  = "UnknownAsset" )
      : m_Metadata( metadata )
      , m_strName( strName )
      , m_pData()
    {
    }

    ~Asset() = default;

    Asset( const Asset & ) = delete;
    Asset( Asset && )      = default;

  public:
    const AssetMetadata &GetMetadata() const
    {
        return m_Metadata;
    }

    const ASSET_TYPE &GetData() const
    {
        return *m_pData;
    }

    const ::std::string &GetName() const
    {
        return m_strName;
    }

  private:
    const AssetMetadata m_Metadata = {};
    const ::std::string m_strName  = "Unknown";
    ASSET_TYPE         *m_pData    = {};
};

// --------------------------------------------------------------------------------------------------------------------
class AssetsManager
{
  public:
    AssetsManager() = default;

    ~AssetsManager()
    {
        ::std::cout << "List of loaded assets:" << ::std::endl;
        for ( auto &vk : m_Assets )
            ::std::cout << vk.first << ::std::endl;
    }

  public:
    template <class ASSET_TYPE>
    ::std::weak_ptr<Asset<ASSET_TYPE>> GenerateAsset( ::std::string_view assetName )
    {
        ::std::cout << "Generating asset" << ::std::endl;

        const auto fullAssetName = ConstructFullAssetNameInternal<ASSET_TYPE>( assetName );

        if ( m_Assets.find( fullAssetName ) != m_Assets.end() )
        {
            return ::std::static_pointer_cast<Asset<ASSET_TYPE>>( m_Assets[ fullAssetName ] );
        }

        auto resultVal            = ::std::make_shared<Asset<ASSET_TYPE>>();
        m_Assets[ fullAssetName ] = resultVal;
        return resultVal;
    }

  private:
    template <class ASSET_TYPE>
    ::std::string ConstructFullAssetNameInternal( ::std::string_view assetName )
    {
        return ::std::string( assetName ) + '_' + ::std::string( ASSET_TYPE::GetAssetTypeName() );
    }

  private:
    ::std::unordered_map<::std::string, ::std::shared_ptr<AssetWrapper>> m_Assets = {};
};

// --------------------------------------------------------------------------------------------------------------------
using namespace std;

int main()
{
    cout << "START___" << endl;
    try
    {
        AssetsManager assetsManager = {};

        auto anAsset      = assetsManager.GenerateAsset<Mesh>( "Test" ).lock();
        auto anotherAsset = assetsManager.GenerateAsset<Mesh>( "Test" ).lock();
        auto diffrentAsset = assetsManager.GenerateAsset<Sound>( "Test" ).lock();

        anAsset->GetData().Action();
        anotherAsset->GetData().Action();

        assert( anAsset.get() == anotherAsset.get() );
    }
    catch ( ... )
    {
    }
    cout << "___END" << endl;
}
